#include <unistd.h>
#include <stdio.h>
int main(){
    printf("Before exec\n");

    execl("/bin/ls","ls",NULL);

    printf("After excel\n");
}