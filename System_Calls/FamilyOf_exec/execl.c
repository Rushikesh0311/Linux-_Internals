#include <stdio.h>
#include <unistd.h>
int main(){

    printf("Before\n");
    execl("/bin/ls","ls","-l",NULL);
    printf("Afer\n"); // this is not printed after the execl --> img has been replaced bt an ls command
}