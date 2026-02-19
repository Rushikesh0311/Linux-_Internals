#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define LOOPBACK_IP "127.0.0.1"

/* Subserver Ports */
#define SERVER_PORT_ADD 6001
#define SERVER_PORT_SUB 6002
#define SERVER_PORT_MUL 6003
#define SERVER_PORT_DIV 6004
#define SERVER_PORT_MOD 6005

#define AGENT_PORT 3000
#define SERVER_LENGTH 5

int main()
{
    char opt;
    int port;

    
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    
    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(AGENT_PORT);
    server_info.sin_addr.s_addr = inet_addr(LOOPBACK_IP);

    
    bind(sock_fd, (struct sockaddr *)&server_info, sizeof(server_info));

    
    listen(sock_fd, SERVER_LENGTH);
    printf("Agent server is listening on port %d...\n", AGENT_PORT);

    while (1)
    {
        
        int data_sockfd = accept(sock_fd, NULL, NULL);
        printf("Connection Established...\n");

        
        recv(data_sockfd, &opt, sizeof(opt), 0);

    
        if(opt == '+')
            port = SERVER_PORT_ADD;
        else if(opt == '-')
            port = SERVER_PORT_SUB;
        else if(opt == '/')
            port = SERVER_PORT_DIV;
        else if(port == '*')
            port = SERVER_PORT_MUL;
        else if(port == '%')
            port = SERVER_PORT_MOD;
        else
            printf("Invalid Oprator...\n");
        
        send(data_sockfd, &port, sizeof(port), 0);

        close(data_sockfd);
    }

    close(sock_fd);
}
