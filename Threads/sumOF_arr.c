#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
struct  sum
{
    int start;
    int end;

};
int arr[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
void *arr_sum(void* arg)
{
    int sum = 0;
    for(int i = ((struct sum*)arg)->start;i<=((struct sum*)arg)->end;i++)
        sum += arr[i];
    return (void*)(long)sum;
}
int main(){
    pthread_t tid1,tid2;
    struct sum my_var[2];
    int sum1,sum2;

    my_var[0].start = 0; my_var[0].end = 9;
    my_var[1].start = 10; my_var[1].end = 19;

    pthread_create(&tid1,NULL,arr_sum,&my_var[0]);
    pthread_create(&tid2,NULL,arr_sum,&my_var[1]);

    pthread_join(tid1,(void**)&sum1);
    pthread_join(tid2,(void**)&sum2);
    printf("Sum %d\n",sum1+sum2);
}

