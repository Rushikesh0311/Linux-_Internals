#include "tftp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// char mode[20];
// int data_size;
void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet);

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    tftp_packet packet;


    // Create UDP socket
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd < 0)
    {
        perror("sockfd");
        exit(EXIT_FAILURE);
    }


    // Set socket timeout option
    //TODO Use setsockopt() to set timeout option
    // Set up server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY; // to accept any ip that user entered


    
    
   

    // Bind the socket
    if(bind(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening on port %d...\n",PORT);
  

    printf("TFTP Server listening on port %d...\n", PORT);

    // Main loop to handle incoming requests
    while (1) {

        client_len = sizeof(client_addr); // resting the len each time 
        int n = recvfrom(sockfd, &packet, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &client_len);
        if (n < 0) {
            perror("Receive failed or timeout occurred");
            continue;
        }

         handle_client(sockfd, client_addr, client_len, &packet);
    }

    close(sockfd);
    return 0;
}

void handle_client(int sockfd, struct sockaddr_in client_addr, socklen_t client_len, tftp_packet *packet) 
{
    // Extract the TFTP operation (read or write) from the received packet
    // and call send_file or receive_file accordingly

    int opcode = ntohs(packet->opcode);

    /* -------------for WRQ-------------- */
    if(opcode == WRQ)
    {
        char *file_name = packet->body.request.filename;

        printf("WRQ received fro file : %s\n",file_name);
        printf("MOde received --> %s\n",packet->body.request.mode);

        strcpy(mode,packet->body.request.mode);

        if(!strcmp(mode,"default"))
            data_size = 512;
        else if(!strcmp(mode,"octet"))
            data_size = 1;
        else if(!strcmp(mode,"netascii"))
            data_size = 128;
        else 
            data_size = 512;

        printf("Server block size sets -->%d\n",data_size);

        int fd = open(file_name,O_CREAT | O_WRONLY | O_EXCL,0644);

        if(fd == -1)
        {
            if(errno == EEXIST)
            {
                fd = open(file_name,O_WRONLY | O_TRUNC);
                if(fd == -1)
                {
                    perror("file fd");
                    return;
                }
            }
            else{
                perror("File open Failed");
                return;
            }
        }
        close(fd);

        tftp_packet ack;
        memset(&ack,0,sizeof(ack));

        ack.opcode = htons(ACK);
        ack.body.ack_packet.block_number = htons(0);

        sendto(sockfd,&ack,sizeof(ack),0,(struct sockaddr*)&client_addr,client_len);

        printf("Sent ACK -->Ready to recive files\n");
        receive_file(sockfd,client_addr,client_len,file_name);
    }
    else if(opcode == RRQ)
    {
        char *file_name = packet->body.request.filename;

        printf("MOde received: %s\n",packet->body.request.mode);
        strcpy(mode,packet->body.request.mode);

        if(!strcmp(mode,"default"))
            data_size = 512;
        else if(!strcmp(mode,"octet"))
            data_size = 1;
        else
            data_size = 512;

        printf("Server block size -->%d\n",data_size);
        printf("RRQ received -->%s\n",file_name);

        int file_fd = open(file_name,O_RDONLY);

        if(file_fd < 0)
        {
            tftp_packet pkt_error;
            memset(&pkt_error,0,sizeof(pkt_error));

            pkt_error.opcode = htons(ERROR);
            pkt_error.body.error_packet.error_code = htons(1);

            strcpy(pkt_error.body.error_packet.error_msg,"File is not existing");

            int lenOf_err = 4 + strlen(pkt_error.body.error_packet.error_msg) + 1;

            sendto(sockfd,&pkt_error,lenOf_err,0,(struct sockaddr*)&client_addr,client_len);
            printf("File Not Found...\n");
            return;
        }
        close(file_fd);
        printf("file Found...Staring Transfer\n");
        send_file(sockfd,client_addr,client_len,file_name);
    }
}




