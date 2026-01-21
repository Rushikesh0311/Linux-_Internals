/*
A08 - WAP to create three child from the same parent
Name - Gore Rushikesh R.
Date - 21-01-026
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
    __pid_t pid = fork();

    if(pid > 0)
    {
        printf("Parent proccess\n");

        __pid_t pid1 = fork();
        if(pid1 > 0)
        {
            pid_t pid2 = fork();
            
            if(pid2 > 0)
            {
                //parent

                 for(int i=1;i<=3;i++)
                {
                    int status;
                    pid_t wait_ret = wait(&status);

                    if(WIFEXITED(status))
                        printf("Child %d terminated with exit code %d\n",wait_ret,WEXITSTATUS(status));
                }
            }
            else if(pid2 == 0)
            {
                printf("Child 3 with PID %d\n",getpid());

                // for(int i=1;i<=3;i++)
                // {
                //     int status;
                //     pid_t wait_ret = waitpid(pid2,&status,WUNTRACED);

                //     if(WIFEXITED(status))
                //         printf("Child %d terminated with exit code %d\n",wait_ret,WEXITSTATUS(status));
                // }
            }
        }
        else if(pid1 == 0)
        {
            printf("Child 2 with PID %d\n",getpid());
            sleep(10);
        }
    }
    else if(pid == 0)
    {
        printf("Child 1 with PID %d\n",getpid());
        sleep(5);
    }
}