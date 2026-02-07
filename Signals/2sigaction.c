#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum,siginfo_t *info,void *argc){
    printf("Signo-> %d\n",info->si_signo);
    printf("Sending procces ID%d\n",info->si_pid);
}
int main(){
    struct sigaction newact = {0};
    printf("PID id %d\n",getpid());

    
    newact.sa_sigaction = own_handler;
    newact.sa_flags = SA_SIGINFO;
    sigaction(SIGINT, &newact, NULL);


    signal(SIGINT,own_handler); // modify action of sigint

   

    while(1);


}