#include <unistd.h>
#include <stdio.h>
#include <string.h>
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
        //close the unused pipe
        close(pipefd[0]);
        char buff[100] = "123qweasdzxc";
        sleep(5);

        //sed data to pipe
        write(pipefd[1],buff,strlen(buff)+1);
    }
    else if(c1 == 0)
    {
        //close unused pipe end
        close(pipefd[1]);
        char buff[100];

        read(pipefd[0],buff,100); // read data from pipe
        printf("Data --> %s\n",buff);

    }
}