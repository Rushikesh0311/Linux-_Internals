#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum){
    printf("Signal %d receiver\n",signum);
}
int main(){
    struct sigaction newact = {0};
    printf("PID id %d\n",getpid());

    
    newact.sa_handler = own_handler;
    sigaction(SIGINT, &newact, NULL);


    signal(SIGINT,own_handler); // modify action of sigint

   

    while(1);


}