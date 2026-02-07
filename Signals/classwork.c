#include <signal.h>
#include <unistd.h>
#include <stdio.h>
void own_handler(int signum){
    static int count = 0;
    count++;

    if(count <= 5)
        printf("Signal %d receiver\n",signum);
    else{
        signal(SIGINT,SIG_IGN); // make procccess ignore the sigint
        _exit(0);
    }
}
int main(){
    printf("PID id %d\n",getpid());

    signal(SIGINT,own_handler); // modify action of sigint
    while(1);


}