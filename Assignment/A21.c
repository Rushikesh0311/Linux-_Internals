/*A21 - WAP to sum and maximum of a given array using multiple threads.Synchronize threads using mutex.*/
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREADS 5
pthread_mutex_t lock_mutext; // Mutex lock variable
int total_sum = 0; // Global variable to store total sum
int max_ele = 0; // store maximum element

struct sum_max
{
    int start;
    int end;
    //int *arr
};
void *sum_and_max(void* argc)
{
    for(int i=((struct sum_max *)argc)->start;i<=((struct sum_max *)argc)->end;i++)
    {
        total_sum += i;

        if(i > max_ele)
            max_ele = i;
    }
}

// Thread function with mutex lock
void* mutex_lock(void *argc)
{
    pthread_mutex_lock(&lock_mutext);

    sum_and_max(argc);

    pthread_mutex_unlock(&lock_mutext);
}
int main(){
    pthread_t tid[5]; // Thread ID array
    struct sum_max my_var[5];  // Structure array for thread data

    int size;
    printf("Enter the Size of the Array: ");
    scanf("%d",&size);

    int Array[size];

    for(int i=0;i<size;i++)// storing 1 to n elements in array
    {
        Array[i]  = i + 1;
    }

    //max_ele = Array[0];

    // for(int i=0;i<THREADS;i++)
    // {
    //     my_var[i].arr = Array;
    // }
    
    // Initialize mutex lock
    pthread_mutex_init(&lock_mutext,NULL);
    int pthread_allocation = size / THREADS;


    int start = Array[0];
    for(int i=0;i<THREADS;i++)
    {
        my_var[i].start = start;

        if(i == THREADS - 1) // Last thread condition
            my_var[i].end = size; // Till end of array
        else
            my_var[i].end = start + pthread_allocation - 1; // Normal range

        start = my_var[i].end + 1; // Update next start

    }
    for(int i=0;i<THREADS;i++)
    {
        pthread_create(&tid[i],NULL,mutex_lock,&my_var[i]);
    }

    for(int i=0;i<THREADS;i++)
    {
        pthread_join(tid[i],NULL);
    }

    printf("Total sum of elements: %d\nMax element from Array: %d\n",total_sum,max_ele);

    pthread_mutex_destroy(&lock_mutext);
   



}