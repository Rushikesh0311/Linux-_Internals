#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h> // permission macros
#include <errno.h>
int main(){
    // int fd = open("f1.txt",O_WRONLY);
    // printf("Fd1 : %d\n",fd);


    //If f3.txt does not exist, create it and open it for writing.
    int fd2 = open("f3.txt",O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
    printf("f2 : %d\n",fd2);


    int fd = open("f5.txt",O_WRONLY);
    printf("fd : %d\n",fd);

    if(fd == -1){
        if(errno == ENOENT) //ENOENT = Error NO ENTry
        printf("File doesn't exist\n");
        else if(errno == EACCES) //EACCES = Access denied
            printf("tHE REQ ACCESS IS NOT ALOOWED\n");
        else if(errno == EEXIST)
            printf("FILE ALEARDY EXISTED\n"); /*EEXIST is returned only with O_CREAT | O_EXCL Your current open() call will never produce EEXIST So this condition is logically unreachable*/
    }

    printf("fd = %d\n",fd);


}