#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
int main(){

    int sock_fd = socket(AF_INET,SOCK_STREAM,0);

    int num1,num2;
    char opt;

    printf("Enter num 1: ");
    scanf("%d",&num1);

    printf("Enter num 2: ");
    scanf("%d",&num2);

    printf("Enter the oprator: ");
    scanf(" %c",&opt);

   


    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(3000);
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock_fd,(struct sockaddr *)&server_info,sizeof(server_info));
    int size = send(sock_fd,&opt,sizeof(opt),0);

    int arthmatic_op_port;
    recv(sock_fd,&arthmatic_op_port,sizeof(arthmatic_op_port),0);
    printf("Received port: %d\n", arthmatic_op_port);


    close(sock_fd);

    int new_sockfd = socket(AF_INET,SOCK_STREAM,0);

    //server_info.sin_family = AF_INET;
    server_info.sin_port = htons(arthmatic_op_port);

    connect(new_sockfd,(struct sockaddr *)&server_info,sizeof(server_info));

    send(new_sockfd,&num1,sizeof(num1),0);
    send(new_sockfd,&num2,sizeof(num2),0);

    int res = 0;
    recv(new_sockfd,&res,sizeof(res),0);
    printf("Result: %d\n",res);

    close(new_sockfd);

    

}
