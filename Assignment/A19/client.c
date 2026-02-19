#include "header.h"

int main()
{
    input_cmd_packet new;
    data_packet data_var;
    ack_packet ack;

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    if(sock_fd == -1)
    {
        perror("socket");
        return 0;
    }

    struct sockaddr_in server_info;
    socklen_t addr_len = sizeof(server_info);

    server_info.sin_port = htons(5001);
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr =inet_addr("127.0.0.1");

    printf("Enter the command: ");
    scanf(" %[^\n]", new.cmd);

    printf("Enter the no. of time ot execute: ");
    scanf("%d", &new.no_of_times);

    // Send command packet
    sendto(sock_fd, &new, sizeof(new), 0,(struct sockaddr *)&server_info,addr_len);

    // File to store received output
    int file_fd = open("client_output.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);

    while(1)
    {
        recvfrom(sock_fd, &data_var,sizeof(data_var), 0,(struct sockaddr *)&server_info,&addr_len);

        if(data_var.flag == 0)
            break;

        write(file_fd,data_var.data,data_var.size);

        ack.packet_no = data_var.packet_num;

        sendto(sock_fd, &ack,sizeof(ack), 0,(struct sockaddr *)&server_info,addr_len);
    }

    printf("File received successfully\n");

    close(file_fd);
    close(sock_fd);
}
