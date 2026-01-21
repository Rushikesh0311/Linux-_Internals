#include <stdio.h>
#include <unistd.h>
int main(){
    __pid_t ret = fork();

    if(ret > 0){
        __pid_t c2 = fork();

        if(c2 > 0)
        {
            printf("This is parent with PID : %d and PPID : %d\n",getpid(),getppid());
        }
        else if(c2 == 0)
            printf("This is child2 with PID : %d and PPID : %d\n",getpid(),getppid());
        else{
            perror("fork");
            return -1;
        }
    }
    else if(ret == 0)
        printf("This is child1 with PID : %d and PPID : %d\n",getpid(),getppid());
    else{
        perror("fork");
        return -1;
    }
}