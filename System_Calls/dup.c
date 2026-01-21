#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
int main(){
    int fd = open("f1.txt", O_WRONLY);

    int backup = dup(1);
    close(1);
    dup(fd);

    printf("print this msg into FILE\n");

    close(1);
    dup(backup);
    printf("Print this msg into STDOUT\n");
    
}