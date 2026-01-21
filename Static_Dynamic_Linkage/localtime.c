#include <stdio.h>
#include <time.h>
int main(){
	
	time_t t = time(NULL);
	struct tm *local = localtime(&t);
	
	printf("Date: %d %d %d\n",local->tm_mday,local->tm_mon + 1,local->tm_year + 1900);

	printf("Day: %d\n",local->tm_wday);

	printf("Time: %d %d %d\n",local->tm_hour,local->tm_min,local->tm_sec);

	



}
