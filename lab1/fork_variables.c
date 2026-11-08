#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

// zmienna globalna
char global_array[6] = {'a', 'b', 'c', 'd', 'e', 'f'};

int main() {
    int pid;
    // zmienna w funkcji main programu
    char a[6] = {'a', 'b', 'c', 'd', 'e', 'f'};

    if ((pid = fork()) == -1) {
        // w przypadku, gdy nie uda się utworzyć procesu
        // potomnego, wypisujemy w konsoli komunikat błędu
        // i kończymy działanie programu (raportując jednak błąd)
        perror("fork() failed");
        exit(1);
    }

    if (pid == 0) {
        /* udało się utworzyć proces potomny.
        Ten blok kodu będzie wykonywany przez proces potomny.*/
        printf("Child process has variable a = ");
        int i;
        for (i = 0; i < 3; i++) {
            global_array[i] = 'a';
            a[i] = 'a';
        }
        printf("%s\n", a);
    }
    else {
        /* równolegle ten blok kodu wykona się przez proces rodzicielski */
        printf("Parent process has variable a = ");
        int i;
        for (i = 3; i < 6; i++) {
            global_array[i] = 'b';
            a[i] = 'b';
        }
        printf("%s\n", a);
    }

    return 0;
}
