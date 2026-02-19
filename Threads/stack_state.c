#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
pthread_attr_t my_attr;
void *thread(void *arg)
{
    for(int i=0;i<10;i++,sleep(1))
        printf("In thread %d\n",i+1);
    return NULL;
}
int main(){
    pthread_t tid;
    pthread_attr_init(&my_attr);

    size_t stack_size;
    
    pthread_attr_setstacksize(&my_attr,16384);
    pthread_attr_getstacksize(&my_attr,&stack_size);

    printf("stack size %lu\n",stack_size);
   


   // pthread_attr_setdetachstate(&my_attr,PTHREAD_CREATE_DETACHED);

    pthread_create(&tid,&my_attr,thread,NULL);

    pthread_join(tid,NULL);

    for(int i=0;i<10;i++,sleep(1))
        printf("In main %d\n",i+1);
    pthread_attr_destroy(&my_attr);
}