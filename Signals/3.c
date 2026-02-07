#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum,siginfo_t *info,void *argc){

    printf("child %d terminated with exit code %d\n",info->si_pid,info->si_status);
}
int main(){
    struct sigaction newact = {0};

    __pid_t pid = fork();

    
    printf("PID id %d\n",getpid());

    if(pid > 0){
    newact.sa_sigaction = own_handler;
    newact.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NOCLDWAIT;
    sigaction(SIGCHLD, &newact, NULL);
    printf("Parent PID: %d\n",getpid());
    sleep(20);
    }
    else if(pid == 0)
    {
        printf("Child with PID: %d\n",getpid());
        sleep(10);
    }


}