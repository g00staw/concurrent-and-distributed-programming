#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pid;

    if ((pid = fork()) == -1) {
        perror("fork() failed");
        exit(1);
    }

    if (pid == 0) {
        /* Proces potomny wypisuje 'Hello, ' */
        printf("Hello, ");
        fflush(stdout); // upewniamy się, że bufor wyjścia został opróżniony
    } else {
        /* Proces rodzicielski czeka na zakończenie procesu potomnego */
        wait(NULL);
        printf("World!\n");
    }

    exit(0);
}
