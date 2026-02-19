#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
int main(){
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);

    if(sock_fd == -1)
    {
        perror("error");
        return -1;
    }

    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(3000);

    server_info.sin_addr.s_addr = inet_addr("127.0.0.1"); // brings binary ot the IP address --> conver the string to the binary

    bind(sock_fd,(struct sockaddr *)&server_info,sizeof(server_info));

    listen(sock_fd,5); // fixing the queue size

    int data_sockfd = accept(sock_fd,NULL,NULL); // acceepting the client request

    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");


    if(data_sockfd == -1)
    {
        perror("accept");
        return -1;

    }
    printf("Connection established successfully\n");

    char buff[100];
    recv(data_sockfd,buff,100,0);
    printf("Client msg--> %s\n",buff);

}