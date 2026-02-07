#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum){
    printf("Signal %d receiver\n",signum);

    signal(SIGINT,SIG_IGN); // make procccess ignore the sigint
}
int main(){
    printf("PID id %d\n",getpid());

    signal(SIGINT,own_handler); // modify action of sigint

   // signal(SIGTSTP,own_handler); // modifying the actions of sigtstp

    while(1);


}