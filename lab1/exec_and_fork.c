#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    char *myargs[4];
    myargs[0] = strdup("git");
    myargs[1] = strdup("clone");
    myargs[2] = strdup("https://github.com/MarcinMrukowicz/CRUDDemoFront");
    myargs[3] = NULL;

    if (fork() == 0) {
        // tworzymy proces programu git i klonujemy repo
        // jednak wykonujemy to jako proces potomny
        execvp(myargs[0], myargs);
    } else {
        // proces rodzicielski oczekuje na zakończenie procesu potomnego
        wait(NULL);
        // przechodzimy do nowoutworzonego folderu
        chdir("CRUDDemoFront");
        // wypisujemy na ekran zawartość tego folderu
        myargs[0] = strdup("ls");
        myargs[1] = strdup("-l");
        myargs[2] = NULL;
        sleep(10);
        execvp(myargs[0], myargs);
    }

    return 0;
}
