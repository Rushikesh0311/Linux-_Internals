/*
A04 - WAP to understand advanced file control system calls
Name - Gore Rushikesh R.
Date - 20-01-026
*/
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

void prime()
{
    for (int i = 0; i <= 15; i++)
    {
        int c = 0;
        for (int j = 2; j <= i / 2; j++)
            if (i % j == 0) c++;

        if (c == 0)
            printf("%d ", i);
    }
    printf("\n");
}

void fibo()
{
    int a = 0, b = 1;
    for (int i = 0; i <= 15; i++)
    {
        printf("%d ", a);
        int c = a + b;
        a = b;
        b = c;
    }
}

// int main(int argc, char *argv[])
// {
//     if (argc == 1)
//     {
//         printf("Insufficient arguments\n");
//         return 0;
//     }

//     close(1);
//     int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);

//     struct flock f = {F_WRLCK, SEEK_SET, 0, 0, 0};

//     if (fork())
//     {
//         fcntl(fd, F_SETLKW, &f);
//         prime();
//         printf("\n");
//         f.l_type = F_UNLCK;
//         fcntl(fd, F_SETLKW, &f);
//     }
//     else
//     {
//         fcntl(fd, F_SETLKW, &f);
//         fibo();
//         f.l_type = F_UNLCK;
//         fcntl(fd, F_SETLKW, &f);
//     }
// }
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Insufficient arguments\n");
        return 0;
    }
    close(1);

    int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY,0644);
    struct flock var;

    int pid = fork();

    if (pid > 0)
    {
        var.l_type = F_WRLCK; // write lock
        var.l_whence = SEEK_SET; // starting
        var.l_start = 0;
        var.l_len = 0; // locking till end
        var.l_pid = getpid(); // parent PID

        fcntl(fd, F_SETLKW, &var);

        prime();
        printf("\n");
        var.l_type = F_UNLCK; // unlocking
        fcntl(fd, F_SETLKW, &var);
    }
    else if (pid == 0)
    {
        var.l_type = F_WRLCK;
        var.l_whence = SEEK_SET;
        var.l_start = 0;
        var.l_len = 0;
        var.l_pid = getpid();

        fcntl(fd, F_SETLKW, &var);

        fibo();
        var.l_type = F_UNLCK;
        fcntl(fd, F_SETLKW, &var);
    }
}
