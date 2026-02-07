/*A06 - WAP to avoid child becoming zombie without blocking parent

Sample Execution - 
A child created with pid 1234
parent is running
parent is running
parent is running . .
............................
Child 1234 terminated normally with exit status 0 parent terminating*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int main(){

    pid_t pid = fork();

    if(pid == 0)
    {
        sleep(5); // child pro
        exit(0);
    }
    printf("A child created with pid %d\n", pid);


    while(1)
    {
        int status;
        __pid_t res = waitpid(pid,&status,WNOHANG);

        if(res == 0)
        {
            printf("Parent is Running\n");
            sleep(2);
           
        }
        else
        {
            if(WIFEXITED(status))
                printf("Child %d terminated normally with exit status %d\n",pid,WEXITSTATUS(status));

            break;;
        }
    }
    printf("Parent terminated\n");
}