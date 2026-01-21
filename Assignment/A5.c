#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    __pid_t res = fork();

    if(res > 0)
    {
        // sleep(20);
        // printf("child orphan\n");

        char buffer[40];
        sprintf(buffer,"/proc/%d/status",res); //  path -> /proc/<child_pid>/status
        int fd = open(buffer,O_RDONLY);

        int lines = 0;

        while(lines != 3)
        {
            int ch;
            read(fd,&ch,1); // sys --> copies 1 byte to user space
            printf("%c",ch);

            if(ch == '\n')
                lines++;
        }

        sleep(10); // parent sleep --> child became Zombie
        lines = 0;
        lseek(fd,SEEK_SET,0);

        
        while(lines != 2)
        {
            int ch;
            read(fd,&ch,1);
            printf("%c",ch);

            if(ch == '\n')
                lines++;
        }

        int pid = fork(); //orphan child

        if(pid > 0){
            //parent sleeps
            sleep(5);
        }
        else if(pid == 0){
            int oldpid = getppid();   //  current parent PID
            sleep(10);
            int new = getppid();

            if(oldpid != new)
                printf("Proccess %d int cleaned by init\n",getpid());
        }
    }
    // else{
    //     printf("Child is running \n");
    //     sleep(10);
    //     printf("Child in zombie state\n");

    //     __pid_t c2 = fork();
    //     if(c2 == 0)
    //     {
    //         int f1 = getppid();
    //         sleep(1);
    //         int f2 = getppid();

            
    //     }
    // }
    else if(res == 0)
        sleep(5);
}