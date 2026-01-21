#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
int main(){
    int fd = open("f4.txt",O_WRONLY);
    printf("%d\n",fd); //3

    close(fd);

    int fd2 = open("f3.txt",O_WRONLY);
    printf("%d\n",fd2); // 3 

}