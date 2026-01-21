#include <stdio.h>
#include <unistd.h>
int main(){
    __pid_t ret = fork();

    if(ret > 0)
        printf("This is parent with PID : %d and PPID : %d\n",getpid(),getppid());
    else if(ret == 0)
        printf("This is child with PID : %d and PPID : %d\n",getpid(),getppid());
    else{
        perror("fork");
        return -1;
    }
}