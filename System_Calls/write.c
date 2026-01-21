#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
int main()
{
    char buf[100] = "Hello World";

    //write(1, "Hello Linux\n", 12); // show output on the terminal


    int fd = open("first.txt", O_WRONLY | O_CREAT, 0644);
   // write(fd,"Rushikesh Gore",14); 

    size_t ret = write(fd,buf,strlen(buf)+1);
    printf("%lu\n",ret);

    
    
}
