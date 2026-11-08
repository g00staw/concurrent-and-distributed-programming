#include <sys/ipc.h> // ipc - inter process communication
#include <sys/shm.h> // shm - shared memory
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#define SHMSIZE 27 // makro, definiujące rozmiar pamięci współdzielonej
int main()
{
    // identyfikator bloku pamięci współdzielonej
    int shmid;
    // te dane będziemy współdzielić
    // w naszym przypadku będzie to pointer, przechowujący znaki
    char *shm;
    if (fork() == 0)
    {
        // proces potomny, alokuje blok pamięci współdzielonej
        // podajemy identyfikator bloku (tutaj arbitralny), rozmiar bloku oraz
        // flagi dostępu do bloku (czy można zapisywać i odczytywać z bloku, itp.)
        shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
        // do zmiennej shm zostanie przypisana zawartość
        // pamięci współdzielonej, o podanym identyfikatorze
        shm = shmat(shmid, 0, 0);
        // rzutowanie zawartości pamięci współdzielonej na
        // typ char*
        char *s = (char *)shm;
        // ustawiamy pierwszy znak w pointerze (chcemy dodawać potem znaki, więc nie może być pusty)
        *s = '\0';
        // proces dodaje znaki do pointera, jednocześnie modyfikując pamięć współdzieloną
        int i;
        for (i = 0; i < 3; i++)
        {
            sprintf(s, "%s%c", s, 'a');
        }
        // wypisujemy na ekran rezultat działania programu
        printf("Child append %s\n", shm);
        // proces potomny zwalnia pamięć współdzieloną
        shmdt(shm);
    }
    else
    {
        // proces rodzica alokuje ten sam blok pamięci współdzielonej,
        // co proces potomny
        shmid = shmget(2009, SHMSIZE, 0666 | IPC_CREAT);
        // do zmiennej shm zostanie przypisana zawartość
        // pamięci współdzielonej, o podanym identyfikatorze
        // formalnie rodzic, nie widzi zmiennej o tej samej nazwie u
        // dziecka
        shm = shmat(shmid, 0, 0);
        // czekamy na zakończenie procesu potomnego
        wait(NULL); // proces rodzica odczytuje teraz zawartość pamięci współdzielonej
        printf("Parent reads %s\n", shm);
        // podobna, operacja, jak u dziecka: dodajemy znaki do łańcucha znaków
        // tutaj łańcuch nie jest już pusty, więc nie musimy go inicjować
        // jest tak, bo rodzic czekał aż proces potomny wykona się
        char *s = (char *)shm;
        int i;
        for (i = 0; i < 3; i++)
        {
            sprintf(s, "%s%c", s, 'b');
        }
        strcat(s, "\n");
        printf("Parent append %s\n", shm);
        // proces rodzicielski zwalnia pamięć współdzieloną
        shmdt(shm);
        // usunięcie segmentu o podanym identyfikatorze
        // zapewnia to opcja RMID (remove id)
        // ta funkcja systemowa ogólnie może wykonywać inne zadania
        shmctl(shmid, IPC_RMID, NULL);
    }
    return 0;
}