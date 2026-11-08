#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIFO_PATH "/tmp/my_first_named_pipe"
#define MAX_BUF 1024

int main(void) {
    int fd = open(FIFO_PATH, O_RDONLY);
    if (fd == -1) {
        perror("open (uruchom najpierw named_pipe_1)");
        return EXIT_FAILURE;
    }

    char buffer[MAX_BUF] = "";
    ssize_t bytes = read(fd, buffer, sizeof buffer - 1);
    if (bytes == -1) {
        perror("read");
        close(fd);
        return EXIT_FAILURE;
    }
    buffer[bytes] = '\0';
    printf("Received: %s\n", buffer);
    close(fd);
    return EXIT_SUCCESS;
}
