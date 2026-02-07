#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main(){
    char buff[200];

    mkfifo("/tmp/myfifo",0644);
    int fd = open("/tmp/myfifo",O_RDONLY);


    read(fd,buff,100);
    printf("Data --> %s\n",buff);
}