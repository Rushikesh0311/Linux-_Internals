#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(){
    int fd = open("dup2.txt", O_WRONLY | O_CREAT | O_EXCL, 0644);

    dup2(fd,1);
    close(fd);
    printf("this will print int dup2.text\n");

}