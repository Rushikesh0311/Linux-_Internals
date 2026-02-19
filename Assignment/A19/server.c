#include "header.h"

int main()
{
    struct sockaddr_in server_info, client_addr;
    input_cmd_packet new;
    data_packet data_var;

    socklen_t addr_len = sizeof(client_addr);

    int sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(5001);
    server_info.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind
    if(bind(sock_fd, (struct sockaddr *)&server_info,sizeof(server_info)) < 0)
    {
        perror("bind");
        exit(1);
    }

    printf("Server is waiting...\n");

    int receive = recvfrom(sock_fd, &new, sizeof(new), 0,(struct sockaddr *)&client_addr,&addr_len);

    if(receive > 0)
    {
        printf("PACKET RECEIVED...\n");
        printf("Cmd: %s\n", new.cmd);
        printf("Count: %d\n", new.no_of_times);
    }
    else
    {
        perror("recvfrom");
    }

    // Clear file before writing
    int clr = open("output.txt",O_WRONLY | O_CREAT | O_TRUNC, 0644);
    close(clr);

    for(int i = 0; i < new.no_of_times; i++)
    {
        pid_t pid = fork();

        if(pid == 0)
        {
            int fd = open("output.txt",O_WRONLY | O_CREAT | O_APPEND, 0644);

            // Redirect BOTH outputs
            dup2(fd, STDOUT_FILENO);
            dup2(fd, STDERR_FILENO);
            close(fd);

            char temp[256];
            strcpy(temp, new.cmd);

            char* argv[20];
            int j = 0;

            char* token = strtok(temp, " ");
            while(token != NULL)
            {
                argv[j++] = token;
                token = strtok(NULL, " ");
            }
            argv[j] = NULL;

            execvp(argv[0], argv);
            exit(1);
        }
        else if(pid > 0)
        {
            wait(NULL);
        }
        else
        {
            perror("fork");
        }
    }

    // Transferring file content
    int file_fd = open("output.txt", O_RDONLY);
    int packet = 1;
    int byte_size;

    while((byte_size = read(file_fd, data_var.data, 64)) > 0)
    {
        data_var.packet_num = packet;
        data_var.flag = 1;
        data_var.size = byte_size;

        while(1)
        {
            sendto(sock_fd, &data_var, sizeof(data_var), 0,(struct sockaddr *)&client_addr,sizeof(client_addr));

            ack_packet ack;

            recvfrom(sock_fd, &ack, sizeof(ack), 0,(struct sockaddr *)&client_addr,&addr_len);

            if(ack.packet_no == packet)
                break;
        }
        packet++;
    }

    data_var.packet_num = packet;
    data_var.flag = 0;
    data_var.size = 0;

    sendto(sock_fd, &data_var, sizeof(data_var), 0,(struct sockaddr *)&client_addr,sizeof(client_addr));

    close(file_fd);
    close(sock_fd);
}
