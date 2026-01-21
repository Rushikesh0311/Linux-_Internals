#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int choice;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open");
        return 1;
    }

    printf("Menu:\n");
    printf("1. dup()\n");
    printf("2. dup2()\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {

        int saved_stdout = dup(1);
        close(1);
        dup(fd);

        printf("Hi from dup()\n");

        dup2(saved_stdout, 1);
        close(saved_stdout);
    }
    else if (choice == 2)
    {
        int saved_stdout = dup(1);

        dup2(fd, 1);
        printf("Hi from dup2()\n");

        dup2(saved_stdout, 1);
        close(saved_stdout);
    }
    else
    {
        printf("Invalid choice\n");
    }

    printf("hello world\n");

    close(fd);
    return 0;
}