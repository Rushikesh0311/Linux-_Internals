#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "tftp.h"
#include "tftp_client.h"

bool validate_ip_addr(char *ip);
bool validate_file_present(char *file_name);

// extern char mode[20];
// extern int data_size;


int main() {
    char command[256];
    tftp_client_t client;
    memset(&client, 0, sizeof(client));  // Initialize client structure

    //default transfer mode and block size
    strcpy(mode, "default");
    data_size = 512;

    // Main loop for command-line interface
    while (1) {
        printf("tftp> ");
        fgets(command, sizeof(command), stdin);

        // Remove newline character
        command[strcspn(command, "\n")] = 0;

        // Process the command
        process_command(&client, command);
    }

    return 0;
}

// Function to process commands
void process_command(tftp_client_t *client, char *command) {
    char *cmd = strtok(command," ");

    if(cmd == NULL)
        return;

    if(!strcmp("help",cmd))
        print_help();
    else if(!strcmp("quit",cmd))
    {
        disconnect(client);
        exit(0);
    }
    else if(!strcmp("connect",cmd))
    {
        char *ip = strtok(NULL," "); // to get the ip that user entered
        bool valid_ip = validate_ip_addr(ip); // validating the ip 

        if(valid_ip)
        {
            connect_to_server(client,ip,PORT);
            printf("Connected to the Server...\n");
        }
        else
        {
            printf("Usage: connet <ip>\n");
        }

    }
    else if(!strcmp("get",cmd))
    {
        char *file_name = strtok(NULL," ");
        get_file(client,file_name);
    }
    else if(!strcmp("put",cmd))
    {
        char *file = strtok(NULL," ");
        bool file_presense = validate_file_present(file);

        if(file_presense)
            put_file(client,file);
        else 
            printf("File is NOT presnt\n");
    }


   
}

// Displays list of supported client commands
void print_help()
{
    printf("\n========== TFTP CLIENT COMMANDS ==========\n");

    printf("connect <IP>     -> Establish connection with server\n");
    printf("get <filename>  -> Download file from server\n");
    printf("put <filename>  -> Upload file to server\n");
    printf("mode            -> Set transfer mode\n");
    printf("bye / quit      -> Close the client\n");

    printf("==========================================\n\n");
}


// This function is to initialize socket with given server IP, no packets sent to server in this function
void connect_to_server(tftp_client_t *client, char *ip, int port) {
    // Create UDP socket

    client->sockfd  = socket(AF_INET,SOCK_DGRAM,0);
  
    // Set up server address
    client->server_addr.sin_family = AF_INET;
    client->server_addr.sin_port = htons(PORT);
    client->server_addr.sin_addr.s_addr = inet_addr(ip);

    strcpy(client->server_ip,ip); //storing to the structure
    client->server_ip[strlen(ip)] = '\0';
    client->server_len = sizeof(client->server_addr); 



    

}

void put_file(tftp_client_t *client, char *filename) {
    // Send WRQ request and send file

    if(client->sockfd < 0)
    {
        printf("NOT connected to the server....\n");
        exit(0);
    }
    send_request(client->sockfd,client->server_addr,filename,WRQ);

}

void get_file(tftp_client_t *client, char *filename) {
    // Send RRQ and recive file 

    //receiving file from the server
    send_request(client->sockfd,client->server_addr,filename,RRQ);
  
}

void disconnect(tftp_client_t *client) {
    // close fd
    close(client->sockfd);
   
}
// Function to send RRQ or WRQ packet to server
void send_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    tftp_packet packet;
    memset(&packet, 0, sizeof(packet)); // Clear packet memory

    packet.opcode = htons(opcode);                  // Convert opcode to network byte order
    strcpy(packet.body.request.filename, filename); // Copy filename
    strcpy(packet.body.request.mode, mode);
    printf("File name is : %s\n", filename);

    if (opcode == WRQ)
        printf("Sending WRQ to server...\n");
    else
        printf("Sending RRQ to server...\n");

    // Send packet to server using UDP
    sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Wait for server response
    receive_request(sockfd, server_addr, filename, opcode);
}
// Function to handle server response
void receive_request(int sockfd, struct sockaddr_in server_addr, char *filename, int opcode)
{
    tftp_packet response;
    socklen_t len = sizeof(server_addr);

    // If Write Request
    if (opcode == WRQ)
    {
        // Wait for ACK from server
        int n = recvfrom(sockfd, &response, sizeof(response), 0, (struct sockaddr *)&server_addr, &len);

        if (n < 0)
        {
            perror("recvfrom failed");
            return;
        }

        // If ACK with block number 0, server is ready
        if (ntohs(response.opcode) == ACK && ntohs(response.body.ack_packet.block_number) == 0)
        {
            printf("Server ready. Starting file upload...\n");
            send_file(sockfd, server_addr, len, filename);
        }

        // If server sends error
        else if (ntohs(response.opcode) == ERROR)
        {
            printf("Server ERROR: %s\n", response.body.error_packet.error_msg);
            return;
        }
        else
        {
            printf("Invalid ACK received\n");
        }
    }

    // If Read Request
    else if (opcode == RRQ)
    {
        printf("Receiving file...\n");
        receive_file(sockfd, server_addr, len, filename);
        printf("File download completed\n");
    }
}

bool validate_ip_addr(char *ip)
{
    if(ip == NULL)
        return false;
    
    struct sockaddr_in server_info;

    int res = inet_pton(AF_INET,ip,&(server_info.sin_addr));

    return (res == 1);
}

bool validate_file_present(char *file_name)
{
    int fd = open(file_name,O_RDONLY);

    if(fd == -1)
        return false;

    close(fd);
    return true;
}
