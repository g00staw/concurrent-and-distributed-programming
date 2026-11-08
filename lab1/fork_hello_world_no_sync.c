#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    // zmienna w której przechowujemy id nowego procesu, nazywanego też procesem potomnym
    int pid;
    /* tworzymy tutaj nowy proces, oczywiście
    jest to proces niniejszego programu.
    Teoretycznie możliwa jest sytuacja, w której
    proces nie zostanie utworzony na skutek błędu
    systemu operacyjnego
    */
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
        printf("Hello, ");
    }
    else {
        /* równolegle ten blok kodu wykona się przez proces rodzicielski */
        printf("World!\n");
    }
    exit(0);
}