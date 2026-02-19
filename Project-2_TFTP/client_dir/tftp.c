/* Common file for server & client */
#include <stdio.h>
#include <fcntl.h>
#include "tftp.h"
#include <string.h>
#include <unistd.h>

char mode[20];
int data_size;


//extern int data_size;
void send_file(int sockfd,struct sockaddr_in client_addr,socklen_t client_len,char *filename) 
{
    tftp_packet data_packet;   // packet to send the data
    tftp_packet ack_packet;    // packet to receive ACK

    // open file in read only mode
    int file_fd = open(filename, O_RDONLY); 

    if(file_fd < 0)
    {
        perror("file open failed");
        return;
    }

    int block_no = 1;   // block number starts from 1 in TFTP
    int bytes_read;    // stores number of bytes read from file

    while(1)
    {
        // clear previous packet data
        // prevents garbage values
        memset(&data_packet, 0, sizeof(data_packet));

        // read file data into packet buffer
        bytes_read =
            read(file_fd,data_packet.body.data_packet.data,data_size);

        if(bytes_read < 0)
        {
            perror("file read error");
            close(file_fd);
            return;
        }

        // set opcode as DATA packet
        data_packet.opcode = htons(DATA);

        // set block number in packet header
        data_packet.body.data_packet.block_number = htons(block_no);

        // send packet -> 4 bytes header + data
        sendto(sockfd,  &data_packet, bytes_read + 4,0,(struct sockaddr *)&client_addr,client_len);

        printf("Send block %d -> %d bytes\n",block_no, bytes_read);

        // loop until correct ACK received
        while(1)
        {
            // clear ACK packet memory
            memset(&ack_packet, 0, sizeof(ack_packet));

            // receive ACK from receiver
            recvfrom(sockfd,&ack_packet,sizeof(ack_packet),0,(struct sockaddr *)&client_addr,&client_len);

            // convert opcode network -> host
            int ack_opcode = ntohs(ack_packet.opcode);

            // convert block number
            int ack_block = ntohs(ack_packet.body.ack_packet.block_number);

            // check ACK validity
            if(ack_opcode == ACK && ack_block == block_no)
                break;   // correct ACK received

            // wrong ACK -> retransmit same block
            printf("Resending block %d\n",block_no);

            sendto(sockfd,&data_packet,bytes_read + 4,0,(struct sockaddr *)&client_addr,client_len);
        }

        // if last packet (< block size)
        // transfer completed
        if(bytes_read < data_size)
            break;

        // move to next block
        block_no++;
    }

    printf("File send successfully...\n");

    // close file after transfer
    close(file_fd);
}


void receive_file(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, char *filename) 
{
    // Implement file receiving logic here

    /*
    Receives file data from sender using TFTP.

   - Receives DATA packets block by block
   - Checks block number
   - Writes data into file
   - Sends ACK for each block
   - Stops when last packet is received*/

    tftp_packet data_packet; // receive data
    tftp_packet ack_packet; // packet --> send ack

    int file_fd = open(filename,O_CREAT | O_WRONLY | O_TRUNC,0644);
    if(file_fd < 0){
        perror("File open failed\n");
        return;
    }

    int expected_block = 1;
    int recv_bytes;

    while(1)
    {
        memset(&data_packet,0,sizeof(data_packet));

        recv_bytes = recvfrom(sockfd,&data_packet,sizeof(data_packet),0,(struct sockaddr *)&client_addr,&client_len);
        if(recv_bytes < 0)
        {
            perror("Recive error");
            close(file_fd);
            return;
        }

        if(ntohs(data_packet.opcode) != DATA)
        {
            printf("Invalid packet received\n");
            continue;

        }

        int block_no = ntohs(data_packet.body.data_packet.block_number);

        if(block_no == expected_block)
        {
            int size = recv_bytes - 4; // payload size

            write(file_fd,data_packet.body.data_packet.data,size);

            memset(&ack_packet,0,sizeof(ack_packet));

            ack_packet.opcode = htons(ACK);
            ack_packet.body.ack_packet.block_number = htons(block_no);

            sendto(sockfd,&ack_packet,sizeof(ack_packet),0,(struct sockaddr*)&client_addr,client_len);

            expected_block++;

            if(size < data_size)
                break;

        }
    }
    printf("File received successfully..\n");
    close(file_fd);
}