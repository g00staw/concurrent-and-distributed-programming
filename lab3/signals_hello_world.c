#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

static void parent_has_executed(int signum) {
    (void)signum;
}

int main(void) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        signal(SIGUSR1, parent_has_executed);
        pause();
        printf("World!\n");
        return EXIT_SUCCESS;
    }

    printf("Hello, ");
    fflush(stdout);
    kill(pid, SIGUSR1);
    wait(NULL);
    return EXIT_SUCCESS;
}
