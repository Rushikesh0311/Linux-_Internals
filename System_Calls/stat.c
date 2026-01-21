#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
int main(int argc,char* argv[]){
    struct stat my_stat;

    int fd = open(argv[1],O_WRONLY);

    if(fstat(fd,&my_stat) == -1){
        perror("fstat");
        return -1;
    }
    printf("Size of %s is %lu\n",argv[1],my_stat.st_size);
    printf("Mode of %s is %o\n",argv[1],my_stat.st_mode);
}