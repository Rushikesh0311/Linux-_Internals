/*
A07 - WAP to create child process to execute command passed through command line

Sample execution: -

1. No args passed (Print usage info)
./exe_child Usage: ./exe_child args...
2. Valid arg. passed
./exe_child date
This is the CHILD process, with id 11612
Wed Apr  4 13:27:19 IST 2012
Child exited with status 0*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
int main(int argc,char* argv[]){
    if(argc == 1)
    {
        printf("Invalid Arguments\n");
        _exit(0);
    }

    pid_t ret = fork();

    if(ret == 0)
    {
        printf("This is the CHILD process, with PID %d\n",getpid());
        int res = execvp(argv[1],&argv[1]);

    }
    else
    {
        int status;
        pid_t res = wait(&status);
        printf("Child terminated with status %d\n",status);
    }

}