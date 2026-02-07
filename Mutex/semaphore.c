#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define NO 10
int res1 = 1,res2 = 1,res3 = 1;
int no;
sem_t count;
void* thread(void* arg)
{
    sem_wait(&count);
    int curr = ++no;

    if(res1)
    {
        res1 = 0;
        for(int i=0;i<5;i++,sleep(1), printf("thread %d t1\n",curr))
        res1 = 1;
    }
    else if(res2)
    {
         res2 = 0;
        for(int i=0;i<5;i++,sleep(1), printf("thread %d t2\n",curr))
        res2 = 1;
    }
    else if(res3)
    {
         res3 = 0;
        for(int i=0;i<5;i++,sleep(1),printf("thread %d t3\n",curr))
        res3 = 1;
    }
    sem_post(&count);
    
    

}
int main(){
    pthread_t tid[NO];

    sem_init(&count,0,3);

    for(int i=0;i<NO;i++)
        pthread_create(&tid[i],NULL,thread,NULL);
    printf("All threads were created\n");
    
    for(int i=0;i<NO;i++)
        pthread_join(tid[i],NULL);
}