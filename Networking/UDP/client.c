#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int sock_fd = socket(AF_INET,SOCK_DGRAM,0);

    if(sock_fd == -1)
    {
        perror("error");
        return -1;
    }

    struct sockaddr_in server_info;
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(3000);

    // sending the connection req to the server
    //connect(sock_fd,(struct sockaddr *)&server_info,sizeof(server_info));

    char buff[100];
    printf("Enter the msg to send to the server: ");
    scanf("%s",buff);

    sendto(sock_fd,buff,strlen(buff),0,(struct sockaddr *)&server_info,sizeof(server_info));

}