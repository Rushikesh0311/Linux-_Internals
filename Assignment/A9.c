
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
int main(int argc , char* argv[])
{

    if(argc < 1)
    {
        printf(" Error: No arguments passed Usage: ./pipe   <command1 > '|'   <command2> ");
    }
    else if(argc < 4)
    {
        printf("Error: Insufficient arguments passed Usage: ./pipe  <command1 > '|'   <command2> ");
    }


    int pipefd[2];
    if(pipe(pipefd) == -1)
    {
        perror("pipe");
        return -1;
    }
    int pos = -1;
    for(int i=0;i<argc;i++)
    {
        if(strcmp(argv[i],"|") == 0)
        {
            
            argv[i] = NULL;
            pos = i+1;
            
            break;
        }
    }
    if(pos == -1)
    {
        printf("Error: '|' not found\n");
        return 1;
    }
    if(pos >= argc)
    {
        printf("Error: No command after '|'\n");
        return 1;
    }

    int child1 = fork();
    if(child1 > 0)
    {
        int child2 = fork();
        if(child2 > 0)
        {
            close(pipefd[0]);
            close(pipefd[1]);
            for(int i=0;i<2;i++)
            {
                wait(NULL);
            }
        }
        else if(child2 == 0)
        {
          close(pipefd[1]); // write end closed
         dup2(pipefd[0],0); // read --> stdin
         execvp(argv[pos],argv+pos);
         printf("Command Not Ruun Properly");
        }
        

    } 
    else if(child1 == 0)
    {
        
        close(pipefd[0]); // read end closed
        dup2(pipefd[1],1); // stdout --> write end
        execvp(argv[1],argv+1);
        printf("Command Not Ruun Properly");
        
    }

}