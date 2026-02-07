#include <stdio.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
// proccess 2 shm contains --> input 
void char_rev(char* ch)
{ 
    int i = 0;
    int j = strlen(ch)-1;
    while(i<=j)
    {

        char temp = ch[i];
        ch[i] = ch[j];
        ch[j] = temp;
        i++;
        j--;
    }
}
int main(){

    char buff[100];

    int shmid = shmget('A',100,0644);
    char *ptr = shmat(shmid,NULL,0);

    strcpy(buff,ptr);
    shmdt(ptr);

    char_rev(buff);

    int fd = open("/tmp/myfifo",O_WRONLY);
    write(fd,buff,strlen(buff)+1); // write in fifo
    close(fd);

}