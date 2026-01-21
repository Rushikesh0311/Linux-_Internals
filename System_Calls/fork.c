#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int main(){
    printf("Before system\n");
    fork();
    printf("After system\n");
}