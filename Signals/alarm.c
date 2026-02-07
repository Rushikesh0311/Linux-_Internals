#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum){
    printf("Signal %d receiver\n",signum);
    printf("Wakeupppp\n");
    alarm(2);
}
int main(){
    printf("PID id %d\n",getpid());

    signal(SIGALRM,own_handler); // modify action of sigint

    //signal(SIGTSTP,own_handler); // modifying the actions of sigtstp

    alarm(5);

    while(1)
        pause();

}