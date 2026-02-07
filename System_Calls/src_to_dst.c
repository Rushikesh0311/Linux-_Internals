#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
int main(){

    int buff[100];
    int src = open("f1.txt",O_RDONLY);

    int dst = open("rushi.txt",O_WRONLY | O_CREAT | O_TRUNC,0644);

    int byte;
    while((byte = read(src,buff,sizeof(buff)))> 0)
    {
        write(dst,buff,byte);
    }
    close(src);
    close(dst);


}