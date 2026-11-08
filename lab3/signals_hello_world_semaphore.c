#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define SEMAPHORE_NAME "/pwir_lab3_hello"

static void parent_has_executed(int signum) {
    (void)signum;
}

int main(void) {
    sem_unlink(SEMAPHORE_NAME); /* usuwa pozostałość po wcześniejszym błędnym uruchomieniu */
    sem_t *ready = sem_open(SEMAPHORE_NAME, O_CREAT | O_EXCL, 0660, 0);
    if (ready == SEM_FAILED) {
        perror("sem_open");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        sem_close(ready);
        sem_unlink(SEMAPHORE_NAME);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        signal(SIGUSR1, parent_has_executed);
        sem_post(ready); /* handler jest już gotowy */
        pause();
        printf("World!\n");
        sem_close(ready);
        return EXIT_SUCCESS;
    }

    printf("Hello, ");
    fflush(stdout);
    sem_wait(ready);
    kill(pid, SIGUSR1);
    wait(NULL);
    sem_close(ready);
    sem_unlink(SEMAPHORE_NAME);
    return EXIT_SUCCESS;
}
