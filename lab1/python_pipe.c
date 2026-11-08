#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(void) {
    int p[2];
    if (pipe(p) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // Proces potomny: przekierowanie stdout do potoku
        dup2(p[1], STDOUT_FILENO);
        close(p[0]); // zamykamy deskryptor do odczytu
        close(p[1]); // zamykamy zduplikowany deskryptor zapisu

        char *args[] = {
            "python3",
            "-c",
            "for i in range(10): print(i)",
            NULL
        };

        execvp(args[0], args);
        perror("execvp python3 failed");
        exit(1);
    } else {
        // Proces rodzicielski: odczyt wyjścia z potoku do bufora
        close(p[1]); // zamykamy deskryptor do zapisu w rodzicu (aby doczekać się EOF)

        char buffer[BUFFER_SIZE];
        ssize_t bytes_read;
        int even_count = 0;
        int total_count = 0;

        printf("--- Odczyt z potoku procesu potomnego (Python) ---\n");

        while ((bytes_read = read(p[0], buffer, BUFFER_SIZE - 1)) > 0) {
            buffer[bytes_read] = '\0';
            printf("Odebrano bufor surowych danych:\n%s", buffer);

            // Przetwarzanie liczb z bufora
            char *line = strtok(buffer, "\r\n ");
            while (line != NULL) {
                int num = atoi(line);
                total_count++;
                if (num % 2 == 0) {
                    even_count++;
                    printf(" -> Liczba parzysta: %d\n", num);
                } else {
                    printf(" -> Liczba nieparzysta: %d\n", num);
                }
                line = strtok(NULL, "\r\n ");
            }
        }

        close(p[0]); // zamykamy deskryptor odczytu
        wait(NULL);  // oczekujemy na zakończenie procesu potomnego

        printf("--------------------------------------------------\n");
        printf("Podsumowanie:\n");
        printf("Liczba wszystkich odebranych liczb: %d\n", total_count);
        printf("Liczba liczb parzystych: %d\n", even_count);
    }

    return 0;
}
