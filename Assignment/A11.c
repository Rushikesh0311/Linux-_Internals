/*
A11 - WAP to implement 'n' pipes to execute 'n+1' commands
Name - Gore Rushikesh
Date - 31-01-026
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc,char* argv[])
{
    if (argc == 1)
    {
        printf("Error: No arguments passed\n");
        printf("Usage: %s <command1> '|' <command2> '|' <command3>\n", argv[0]);
        return 1;
    }

    
    // req --> cmd1 | cmd2 | cmd3
    if (argc < 6)
    {
        printf("Error: Insufficient arguments passed\n");
        printf("Usage: %s <command1> '|' <command2> '|' <command3>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    int cmd_idx[10];
    int idx = 0;

    int prev_pipefd[2];

    cmd_idx[idx++] = 1; 
    for(int i=1;i<argc;i++)
    {
        if(argv[i][0] == '|')// argv[i] = | --> argv[i+1] = cmd
        {
            argv[i] = NULL;
            cmd_idx[idx] = i + 1;
            idx++;
        }
    }

    for(int i=0;i<idx;i++)
    {
        if(i != (idx - 1))
            pipe(pipefd);

        pid_t pid = fork();

        if(pid == 0)
        {
            if(i != 0) // not first cmd --> reading from prev pipe
            {
                dup2(prev_pipefd[0],0); // redirect ---> stdin
                close(prev_pipefd[0]); // closed read end
                //close(prev_pipefd[1]); // closed write end
            }

            if(i != idx - 1)
            {
                dup2(pipefd[1],1); //stdout
                close(pipefd[0]);  // read end
                close(pipefd[1]);   // close write end
            }
            execvp(argv[cmd_idx[i]],(argv + cmd_idx[i]));
            _exit(1);
        }
        else
        {
            if(i != 0)
            {
                close(prev_pipefd[0]);
                //close(prev_pipefd[1]);
            }

             // close prev pipe read end 
            // if (i > 0)
            //     close(prev_pipefd[0]);

            if(i != idx - 1)
            {
                prev_pipefd[0] = pipefd[0]; // read
               // prev_pipefd[1] = pipefd[1]; // write
                close(pipefd[1]);   // closed write end
            }
           
        }
       
    }
    for(int i=0;i<idx;i++)
        wait(NULL);
}