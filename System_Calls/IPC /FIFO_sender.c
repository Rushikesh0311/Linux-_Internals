#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
int main(){
    char str[200];

    mkfifo("/tmp/myfifo",0644);
    int fd = open("/tmp/myfifo",O_WRONLY);


    printf("Enter the string: ");
    scanf("%[^\n]",str);

    write(fd,str,strlen(str)+1);
}