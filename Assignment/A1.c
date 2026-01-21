/*
Assignment - 1
Name - Gore Rushikesh
batch - 25021 + 22

*/
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int src, dst;
    char buf[100];
    int no;
    char ch;

    if(argc < 3 || argc > 4) {
        printf("Usage: %s <src> <dst>\n", argv[0]);
        return 1;
    }

    if(argc == 3 || argc == 4)
    {
        if(argc == 4 && strcmp(argv[1],"-p"))
            src = open(argv[2], O_RDONLY);
        else   
            src = open(argv[1],O_RDONLY); 
        if(src == -1) {
            perror("Source");
            return 1;
        }

        if(argc == 4 && strcmp(argv[1],"-p"))
            dst = open(argv[3], O_WRONLY | O_CREAT | O_EXCL, 0644);
        else
             dst = open(argv[2], O_WRONLY | O_CREAT | O_EXCL, 0644);

        if(dst == -1) {
            printf("Already File exists. Overwrite-> (y/n): ");
            scanf(" %c", &ch);

            if(ch != 'y') {
                close(src);
                return 0;
            }
            if(argc == 4 && strcmp(argv[1],"-p"))
                dst = open(argv[3], O_WRONLY | O_TRUNC);
            else
                dst = open(argv[2], O_WRONLY | O_TRUNC);

        }

        while((no = read(src, buf, sizeof(buf))) > 0)
            write(dst, buf, no);
    }
    close(src);
    close(dst);

    return 0;
}
