#include <stdio.h>
#include <unistd.h>
int main(){
    printf("PID : %d Parent : %d\n",getpid(),getppid());
}