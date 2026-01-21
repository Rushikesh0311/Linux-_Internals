#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

int main() {
    int fd = open("test.txt", O_CREAT | O_WRONLY, 0644);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    // Change permission to rw-------
    fchmod(fd, S_IRUSR | S_IWUSR); // user read write

    close(fd);
    return 0;
}
