#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (fork() == 0) {
        execvp(argv[1], &argv[1]);
    }
}
/*
gcc execvp.c -o my_exec

./my_exec ls -l --->

total 40
-rwxr-xr-x 1 rushi0311 rushi0311 16008 Jan 14 06:44 a.out
-rw-r--r-- 1 rushi0311 rushi0311   103 Jan 14 06:19 execv.c
-rw-r--r-- 1 rushi0311 rushi0311   155 Jan 14 06:44 execvp.c
-rwxr-xr-x 1 rushi0311 rushi0311 16008 Jan 14 06:45 my_exec


./my_exec ls --->
a.out  execv.c  execvp.c  my_exec

./my_exec date   --->
Wed Jan 14 06:48:52 UTC 2026

*/