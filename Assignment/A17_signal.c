/*A17 - WAP to avoid child become zombie by using signals*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
void my_handler(int signum)
{
    int status;
    waitpid(-1,&status,WNOHANG);

    if(WIFEXITED(status))
        printf("Child terminated with exit status %d\n",WEXITSTATUS(status));
    
}
int main(){

    signal(SIGCHLD,my_handler);

    __pid_t pid = fork();
    if(pid == 0)
    {
        sleep(5);
    }
    else if(pid > 0)
    {
        pause(); // pause until the asignal occurs
    }
}