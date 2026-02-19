/*A14 - WAP to print the address which is causing Segmentation fault*/
#include <stdio.h>
#include <sys/signal.h>
#include <stdlib.h>
#include <string.h>
void my_handler(int signum,siginfo_t *info,void *ptr)
{
    // print error msg
    printf("Segmentation fault occurred\n");

    //address
    printf("Fault Add: %p\n",info->si_addr);

    exit(0);
}
int main(){
    // declare sigaction structure
    struct sigaction new_act;

    // struct initialization
    memset(&new_act,0,sizeof(new_act));

    //Assign handler 
    new_act.sa_sigaction = my_handler;

    //enable siginfo
    new_act.sa_flags = SA_SIGINFO;

    //register the SIGSEGV 
    sigaction(SIGSEGV,&new_act,NULL);

    //seg fault
    char *s = "Rushi";
    s[0] = 'H';


}