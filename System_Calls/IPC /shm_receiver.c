#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
int main(){
    char buff[100];

    int shmid = shmget('A',100,0644);

    char *ptr = shmat(shmid,NULL,0);

    strcpy(buff,ptr);
    printf("Data - > %s\n",buff);

    shmdt(ptr);

    shmctl(shmid,IPC_RMID,NULL);
    

}