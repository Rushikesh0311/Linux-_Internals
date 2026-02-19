/*
 1 get current time in sec time()
 2 if date is present or not
    --> NOT
        using localtime(),get curr time int struct C_struct

    --> YES
        convert  string to structure (strptime())
        convert string (time) to structure (strptime())
        find diff --> input_time - curr_time
        if --> dif is +ve
            register alarm signal
            call alarm() , diff
            using pause(), wait for signal
*/
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
char *strptime(const char *s,
               const char *format,
               struct tm *tm);
void my_handler(int signum)
{
    int choice,snooze;

    while(1)
    {
        printf("Alarm Ringing....\n");
        printf("1. Snooze\n");
        printf("2. Exit\n");

        printf("Enter your choice: ");
        scanf("%d",&choice);

        if(choice == 1)
        {
            printf("Enter the snooze time: ");
            scanf("%d",&snooze);

            int seconds = snooze * 60;
            printf("Next alarm in %d min...\n",snooze);

            alarm(seconds); // reseting alarm for snooze
            //pause();
            sleep(seconds);
            printf("Alram ringing again after snoozeee\n");
        }
        else
        {
            printf("Exiting Alarm\n");
            exit(0);
        }
    }
}
int main(){
    time_t curr_time,user_time; // Stores time in seconds

    struct tm *curr_tm; // Pointer to current time struct
    struct tm user_tm; // Struct for user alarm time


    char input_time[20]; // Stores user input string


    signal(SIGALRM,my_handler);

    curr_time = time(NULL); // get the current time in secc
    curr_tm = localtime(&curr_time); // converts second to local time

    user_tm = *curr_tm; // copyed structure

    printf("Currect time %d:%d:%d\n",curr_tm->tm_hour,curr_tm->tm_min,curr_tm->tm_sec);

    printf("Enter the alram :");
    scanf("%s",input_time);

   // conversion string to structure
    // if(strptime(input_time,"%H:%M:%S",&user_tm) == NULL)
    // {
    //     printf("Invalid Timme Format\n");
    //     return 0;
    // }

  
    //date
    // user_tm.tm_mday = curr_tm->tm_mday;
    // user_tm.tm_mon = curr_tm->tm_mon;
    // user_tm.tm_year = curr_tm->tm_year;

    // user_tm.tm_hour = hour;
    // user_tm.tm_min = min;
    // user_tm.tm_sec = sec;

    if(strchr(input_time,' ') != NULL)
    {
        if(strptime(input_time,"%d:%m:%Y %H:%M:%S",&user_tm) == NULL)
        {
            printf("Invalid Date or Time format\n");
            return 0;
        }
    }
    else
    {
        if(strptime(input_time,"%H:%M:%S",&user_tm) == NULL)
        {
            printf("Invalid time format\n");
            return 0;
        }
    }

    user_tm.tm_isdst = -1;

    user_time = mktime(&user_tm);
    int diff = user_time - curr_time;

    if(diff > 0)
    {
        printf("Alram in %d seconds...\n",diff);
        alarm(diff);
        pause(); // waiting for signal
    }
    else
    {
        printf("Invalid time\n");
        return 0;
    }
}