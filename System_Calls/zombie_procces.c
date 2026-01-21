#include <stdio.h>
#include <unistd.h>
int main(){
    __pid_t ret = fork();

    if(ret > 0){
        printf("This is parent with PID : %d and PPID : %d\n",getpid(),getppid());
        //sleep(2); ---> orphan procces ---> later Init will adopt it ---> terminate with Init 
        sleep(20);
        printf("This is parent with PID : %d and PPID : %d\n",getpid(),getppid());
        printf("Parent termination\n");
    }
    else if(ret == 0){
        printf("This is child with PID : %d and PPID : %d\n",getpid(),getppid());
        sleep(5);
        printf("This is child with PID : %d and PPID : %d\n",getpid(),getppid());
        printf("Child termination\n");

    }
    else{
        perror("fork");
        return -1;
    }
}