#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
int main(){
    int pipefd[2];

    if(pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    printf("pipefd[0] -> %d\n",pipefd[0]);
    printf("pipefd[0] -> %d\n",pipefd[1] );

    __pid_t c1 = fork();

    if(c1 > 0)
    {
        __pid_t child2 = fork();
        if(child2 > 0)
        {
            close(pipefd[0]);
            close(pipefd[1]);

            //wait for child 2
            for(int i=0;i<2;i++)
                wait(NULL);
        }
        else if(child2 == 0)
        {
            close(pipefd[1]);

            char buf[100];
            //read(pipefd[0],buf,100);
            //("Data --> %s\n",buf);

            dup2(pipefd[0],0);
            execl("/bin/wc","wc",NULL);
        }

    }
    else if(c1 == 0)
    {
         //close the unused pipe
        close(pipefd[0]);
        char buff[100] = "123qweasdzxc";
        sleep(5);

        //sed data to pipe
        //write(pipefd[1],buff,strlen(buff)+1);

        dup2(pipefd[1],1);
        execl("/bin/ls","ls",NULL);

    }
}