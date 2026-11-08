#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>

#define KEY 12345
#define WAITING_FOR_USER2 1
#define WAITING_FOR_USER1 0

struct shared_memory {
    char buffer[100];
    int turn;
    pid_t pid1;
    pid_t pid2;
};

static struct shared_memory *shared;

static void receive_from_user1(int signum) {
    (void)signum;
    printf("Received from User1: %s", shared->buffer);
    fflush(stdout);
}

int main(void) {
    int shmid = shmget(KEY, sizeof *shared, IPC_CREAT | 0666);
    if (shmid == -1 || (shared = shmat(shmid, NULL, 0)) == (void *)-1) {
        perror("shared memory");
        return EXIT_FAILURE;
    }

    shared->pid2 = getpid();
    shared->turn = WAITING_FOR_USER2; /* user2 rozpoczyna rozmowę */
    signal(SIGUSR2, receive_from_user1);
    while (shared->pid1 == 0)
        usleep(1000);

    for (;;) {
        while (shared->turn != WAITING_FOR_USER2)
            pause();

        printf("User2: ");
        fflush(stdout);
        if (!fgets(shared->buffer, sizeof shared->buffer, stdin) ||
            strcmp(shared->buffer, "q\n") == 0)
            break;

        shared->turn = WAITING_FOR_USER1;
        kill(shared->pid1, SIGUSR1);
    }

    shmdt(shared);
    return EXIT_SUCCESS;
}
