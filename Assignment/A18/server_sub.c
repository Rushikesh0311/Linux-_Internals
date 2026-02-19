#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOOPBACK_IP "127.0.0.1"
#define SUB_SERVER_PORT 6002

int main()
{
    int num1, num2, res;

    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    printf("Add Server Socket Created...\n");

    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(SUB_SERVER_PORT);
    server_info.sin_addr.s_addr = inet_addr(LOOPBACK_IP);

    bind(sock_fd, (struct sockaddr *)&server_info, sizeof(server_info));

    listen(sock_fd, 1);
    printf("Add server is listening...\n");

    int data_sockfd = accept(sock_fd, NULL, NULL);
    printf("Accepted connection\n");

    recv(data_sockfd, &num1, sizeof(num1), 0);
    recv(data_sockfd, &num2, sizeof(num2), 0);

    res = num1 - num2;

    printf("Client Data: %d %d -\n", num1, num2);
    printf("Result: %d\n", res);

    send(data_sockfd, &res, sizeof(res), 0);

    close(data_sockfd);
    close(sock_fd);
}
