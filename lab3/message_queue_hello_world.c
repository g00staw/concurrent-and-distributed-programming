#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

#define QUEUE_KEY 1234

struct message {
    long type;
    char text[100];
};

int main(void) {
    int queue_id = msgget(QUEUE_KEY, IPC_CREAT | 0666);
    if (queue_id == -1) {
        perror("msgget");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        msgctl(queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    if (pid == 0) {
        struct message message;
        if (msgrcv(queue_id, &message, sizeof message.text, 0, 0) == -1) {
            perror("msgrcv");
            return EXIT_FAILURE;
        }
        if (strcmp(message.text, "wrote") == 0)
            printf("world!\n");
        return EXIT_SUCCESS;
    }

    printf("Hello ");
    fflush(stdout);
    struct message message = {.type = 1};
    strcpy(message.text, "wrote");
    if (msgsnd(queue_id, &message, sizeof message.text, 0) == -1) {
        perror("msgsnd");
        msgctl(queue_id, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    wait(NULL);
    msgctl(queue_id, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
