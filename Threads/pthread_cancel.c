#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
void* thread(void* arg){
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,NULL); // changing cancel req

    //pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL); // changing the cancellation state
    while(1);
}
int main(){
    pthread_t tid;

    pthread_create(&tid,NULL,thread,NULL);

    sleep(4);
    printf("Cancel req\n");
    pthread_cancel(tid);

    //wait for thread
    pthread_join(tid,NULL);
}