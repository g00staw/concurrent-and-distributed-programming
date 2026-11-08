#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(void) {
    char *param1[] = {"ls", NULL};
    char *param2[] = {"wc", NULL};
    int p[2]; // ta zmienna identyfikuje odpowiednio wejście i wyjście z pipe'a
    pipe(p); // tworzymy programowo pipe
    pid_t process1, process2; // pid procesów

    if ((process1 = fork()) == 0) { // tworzymy proces potomny
        dup2(p[1], STDOUT_FILENO); // przekierowujemy jego standardowe wyjście do pipe'a
        // wykorzystując komendę duplicate
        close(p[0]); // ten proces nie korzysta ze standardowego wejścia pipe'a
        execvp("ls", param1); // wywołujemy proces programu ls
        perror("execvp ls failed"); // obsługa ewentualnego błędu
        exit(1);
    } else if (process1 == -1) {
        exit(1); // nieudany fork
    }

    close(p[1]); // proces rodzicielski nie pisze na wyjście pipe'a

    if ((process2 = fork()) == 0) { // tworzymy drugi proces potomny
        dup2(p[0], STDIN_FILENO); // przekierowujemy standardowe wejście z pipe'a
        close(p[1]); // ten proces nie korzysta ze standardowego wyjścia pipe'a
        execvp(param2[0], param2); // wywołujemy proces programu wc
        perror("execvp wc failed"); // obsługa ewentualnego błędu
        exit(1);
    } else if (process2 == -1) { // nieudany fork
        close(p[0]); // upewniamy się że nikt nie czyta z pipe'a
        wait(NULL); // czekamy na zakończenie 1. procesu
        exit(1);
    }

    close(p[0]); // proces rodzicielski sam nie odczytuje wejścia pipe'a
    wait(NULL);
    wait(NULL); // oczekujemy na zakończenie obydwu procesów potomnych
    return 0;
}
