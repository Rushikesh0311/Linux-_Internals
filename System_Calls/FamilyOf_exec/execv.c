#include <unistd.h>

int main() {
    char *args[] = {"ls", "-l", NULL};
    execv("/bin/ls", args);
}
