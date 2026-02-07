#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
int main(){  // sender proccess
    char str[100];

    int shmid = shmget('A',100,IPC_CREAT | 0644); // created shm 

    char* ptr = shmat(shmid,NULL,0); // attaching the shm 


    printf("Enter the String: ");
    scanf("%[^\n]",str);

    
    for(int i=0;str[i] != '\0';i++)
    {
        str[i] = toupper(str[i]);
    }

    strcpy(ptr,str); //write to shm
    printf("Write in shm: %s\n",ptr);

    sleep(10);
    // while(ptr == 0);

    printf("Reading from shm: %s\n",ptr);
    shmdt(ptr);

   
    
    

     




    
}