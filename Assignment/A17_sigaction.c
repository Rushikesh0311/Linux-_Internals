/*A17 - WAP to avoid child become zombie by using signal handlers*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
void sigaction_handler(int signum,siginfo_t *info,void* argc)
{
    if(signum == SIGCHLD)
        printf("Child terminated with exit code %d\n",info->si_status);
}
int main(){
    struct sigaction newact = {0};

    newact.sa_sigaction = sigaction_handler;
    newact.sa_flags = SA_SIGINFO | SA_NOCLDWAIT;

    sigaction(SIGCHLD,&newact,NULL);
   

    __pid_t pid = fork();
    if(pid >0)
    {
        sleep(5);
    }

    else if(pid == 0)
        sleep(2);

}