/*
parent --> pipe --> child --> shm --> proccess2 --> fifo --> output in proccess1
*/
#include <unistd.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
// proccess 1
int main(){ 
    int pipe_fd[2];
   
    
    char str[100];
    pipe(pipe_fd); // pipe between -->parent and child 

    pid_t pid = fork();
    mkfifo("/tmp/myfifo",0644); // fifo created

    if(pid == 0)
    {
        char buff[100];
        close(pipe_fd[1]);

        read(pipe_fd[0],buff,100); // reading from pipe

        for(int i=0;buff[i] != '\0';i++)
        {
            buff[i] = toupper(buff[i]);
        }

        // shm -->create + attach
        int shmid = shmget('A',100,IPC_CREAT | 0644);
        char* ptr = shmat(shmid,NULL,0);

        strcpy(ptr,buff);
        shmdt(ptr);
    }
    else if(pid > 0)
    {
       
        char buff[100];

        close(pipe_fd[0]); // parents read end

        printf("Enter the string: ");
        scanf("%[^\n]",str);

        write(pipe_fd[1],str,strlen(str)+1); // writing in pipe
        close(pipe_fd[1]);

        int fd = open("/tmp/myfifo",O_RDWR); // parent waits here till p2 excutes
        read(fd,buff,100); // child reading from fifo

        printf("Data --> %s\n",buff);
        close(fd);
        
    }

}