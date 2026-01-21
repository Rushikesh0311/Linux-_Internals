#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(){
    __pid_t ret = fork();

    if(ret > 0){
        printf("This is parent with PID : %d and PPID : %d\n",getpid(),getppid());

        int status;
        //pid_t wait_ret = wait(&status);
        pid_t wait_ret = waitpid(ret,&status,WUNTRACED);

        if(WIFEXITED(status))
            printf("Child %d terminated normally with exit code %d\n",wait_ret,WEXITSTATUS(status));
        else
            printf("Child %d terminated abnormally with exit status %d\n",wait_ret,status);
        printf("Parent terminated\n");
    }
    else if(ret == 0)
        printf("This is child with PID : %d and PPID : %d\n",getpid(),getppid());
    else{
        perror("fork");
        return -1;
    }
}