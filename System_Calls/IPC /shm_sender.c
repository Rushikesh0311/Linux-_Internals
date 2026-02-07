#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
int main(){
    char str[100];

    int shmid = shmget('A',100,IPC_CREAT | 0644);

    char *ptr = shmat(shmid,NULL,0);

    printf("Enter the string: ");
    scanf("%[^\n]",str);

    strcpy(ptr,str);

    shmdt(ptr);
}