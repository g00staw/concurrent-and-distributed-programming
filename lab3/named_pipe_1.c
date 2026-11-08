#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_first_named_pipe"

int main(void) {
    if (mkfifo(FIFO_PATH, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo");
        return EXIT_FAILURE;
    }

    int fd = open(FIFO_PATH, O_WRONLY);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    const char message[] = "Hello world named pipe!";
    if (write(fd, message, sizeof message) == -1)
        perror("write");
    close(fd);
    unlink(FIFO_PATH);
    return EXIT_SUCCESS;
}
