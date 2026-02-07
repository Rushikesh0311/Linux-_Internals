#include <pthread.h>
#include <stdio.h>
int count;
void* thread(void* arg)
{
    while(1)
    {
        count = 0;
        count += 5;
        count += 10;
        printf("Count %d\n",count);
    }
    return NULL;
}
int main(){
    pthread_t tid1,tid2;

    pthread_create(&tid1,NULL,thread,NULL);
    pthread_create(&tid1,NULL,thread,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
}