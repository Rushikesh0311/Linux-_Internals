#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define THREADS 3
struct fact{
    int start;
    int end;
};
void* factorial(void *argc)
{
    long long sum = 1;
    for(int i = ((struct fact*)argc)->start;i<=((struct fact*)argc)->end;i++)
        sum *= i;
    return (void*)(long)sum;
}
int main(){
    pthread_t tid[3];
    struct fact my_var[3];

    int num;
    printf("Enter the number: ");
    scanf("%d",&num);

    //int sum1,sum2,sum3;

    // for(int i=0;i<THREADS;i++)
    // {
    //     my_var[i].start = i + 1;
    //     my_var[i].end = num;
    //     pthread_create(&tid[i],NULL,factorial,&my_var[i]);
    // }

    int th_allocation = num / THREADS;
    for(int i=0;i<THREADS;i++)
    {
        my_var[i].start = i * th_allocation + 1;

        if(i == THREADS - 1)
            my_var[i].end = num;
        else
            my_var[i].end = (i + 1) * th_allocation;
        pthread_create(&tid[i],NULL,factorial,&my_var[i]);
    }

    long long res = 1;

    for(int i=0;i<THREADS;i++)
    {
        void *ret;
        pthread_join(tid[i],&ret);
        res *= (long long)ret;
    }
    printf("Facotial of %d: %lld\n",num,res);
   



}