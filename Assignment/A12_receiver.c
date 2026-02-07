#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
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
int main(){  // receiver proccess
    char buff[100];


    int shmid = shmget('A',100,0644);
    char* ptr = shmat(shmid,NULL,0);

    printf("Read from shm: %s\n",ptr);

    

    char_rev(ptr); // reversing the string

    //strcpy(ptr,buff); // str sends to the shm
    printf("rev + write --> %s\n",ptr);

    shmdt(ptr); // deattached the shm

    
    




}