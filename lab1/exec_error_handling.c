#include <stdlib.h> // biblioteka standardowa języka C
#include <unistd.h> // biblioteka zawierająca api biblioteki POSIX
#include <string.h>
#include <stdio.h> // standard input output library
#include <errno.h> // biblioteka z definicjami błędów wykonania programu

/*
demonstracja jednego z wariantów funkcji exec
utworzony zostanie proces innego programu
po czym na ekran zostanie wypisany rezultat
jego działania
*/
int main() {
    char *myargs[3]; // 3-elementowa tablica wskaźników na typ char
    myargs[0] = strdup("wc"); // wywołamy program wc (word count), zatem podajemy jego nazwę jako 1. argument
    myargs[1] = strdup("exec_example.c"); // nazwa pliku, gdzie chcemy zliczyć słowa, zliczamy słowa występujące w tym pliku!, 2. argument
    myargs[2] = NULL; // oznaczenie końca tablicy, musi tak być zgodnie z dokumentacją

    int status = execv(myargs[0], myargs); // uruchamiamy proces wc, z argumentem exec_example.c
    printf("%s %i \n", "Status of executing process:", status);

    if (status == -1) {
        // pobieramy numer błędu
        int errsv = errno;
        if (errsv == ENOENT) { // Error no entry
            printf("%s%s\n", "The is no such file as ", myargs[0]); // komunikat o błędzie
        }
    }

    printf("%s\n", "I will be printed, because exec will fail.");
    exit(0); // kończymy wykonanie programu z poprawnym statusem wykonania
}