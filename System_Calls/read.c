#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    char buff[100];
    int fd = open("first.txt",O_RDONLY);

    ssize_t ret;

    do{
        ret = read(fd,buff,20);
        printf("ret -> %lu\n",ret);
    }while(ret == 20);
}