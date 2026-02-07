#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum,siginfo_t *info,void *argc){
    printf("signal received %d\n",info->si_signo);
   
    int i=0;
    while(i--)
    {
        printf("In handler %d times\n",10-i);
        sleep(1);
    }
}
int main(){
    struct sigaction newact = {0};
    printf("PID id %d\n",getpid());

    
    newact.sa_sigaction = own_handler;
    newact.sa_flags = SA_SIGINFO;
   


    sigaddset(&newact.sa_mask,SIGINT);
    sigaddset(&newact.sa_mask,SIGTSTP);
    sigaction(SIGINT,&newact,NULL);
    sigaction(SIGTSTP,&newact,NULL);



    while(1);


}