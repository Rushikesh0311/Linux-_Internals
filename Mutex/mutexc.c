#include <pthread.h>
#include <stdio.h>
int count;
pthread_mutex_t my_lock;
void* thread(void* arg)
{
    while(1)
    {
        pthread_mutex_lock(&my_lock);
        count = 0;
        count += 5;
        count += 10;
        printf("Count %d\n",count);
        pthread_mutex_unlock(&my_lock);
    }
    return NULL;
}
int main(){
    pthread_t tid1,tid2;

    pthread_mutex_init(&my_lock,NULL);
    
    pthread_create(&tid1,NULL,thread,NULL);
    pthread_create(&tid1,NULL,thread,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    pthread_mutex_destroy(&my_lock);
}