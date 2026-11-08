#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h> // mman - memory management

int main()
{
    int pid;
    // zmienna która nie będzie współdzielona w funkcji main programu
    static char a[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    // zmienna, która będziemy współdzielić
    static char *shared_variable;
    // alokujemy pamięć współdzieloną za pomocą funkcji mmap
    // wartość null oznacza, że system operacyjny sam przydzieli adres
    // bloku pamięci, ustawiamy rozmiar bufora, następnie flagi dostępu
    // tutaj zezwalamy na zapis i odczyt z pamięci oraz stwierdzamy, że
    // pamięć może być dzielona, a nie jest prywatnym zasobem procesu
    shared_variable = mmap(NULL, sizeof(char) * 6, PROT_READ | PROT_WRITE,
                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    // ustawiamy wartość początkową zmiennej
    shared_variable[0] = 'a';
    shared_variable[1] = 'b';
    shared_variable[2] = 'c';
    shared_variable[3] = 'd';
    shared_variable[4] = 'e';
    shared_variable[5] = 'f';

    if ((pid = fork()) == -1)
    {
        // w przypadku, gdy nie uda się utworzyć procesu
        // potomnego, wypisujemy w konsoli komunikat błędu
        // i kończymy działanie programu (raportując jednak błąd)
        perror("fork() failed");
        exit(1);
    }

    if (pid == 0)
    {
        // proces potomny zmienia zarówno zmienną
        // a, jak i zmienną współdzieloną
        printf("%s", "Child process has variable a = ");
        int i = 0;
        for (i = 0; i < 3; i++)
        {
            shared_variable[i] = (char)97;
            a[i] = (char)97;
        }
        printf("%s\n", a);
        printf("%s", "Child process has shared variable = ");
        printf("%s\n", shared_variable);
    }
    else
    {
        // proces rodzicielski zmienia zarówno zmienną
        // a, jak i zmienną współdzieloną
        wait(NULL);
        printf("%s", "Parent process has variable a = ");
        int i = 3;
        for (i = 3; i < 6; i++)
        {
            shared_variable[i] = (char)98;
            a[i] = (char)98;
        }
        printf("%s\n", a);
        printf("%s", "Parent process has shared variable = ");
        printf("%s\n", shared_variable);
    }
    printf("%s", "Shared variable outside process block = ");
    printf("%s\n", shared_variable);
    exit(0);
}
