#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h> // mman - memory management
#include <fcntl.h>
#include <string.h>

int main()
{
    // ta zmienna będzie przechowywać odniesienie do pamięci współdzielonej
    static char *shared_variable;
    // tworzymy file descriptor z właściwymi uprawnieniami
    int fd = open("./temp_mmap.txt", O_RDWR | O_CREAT, (mode_t)0600);
    // mapujemy pamięć współdzieloną na utworzony wcześniej plik
    shared_variable = (char *)mmap(NULL, sizeof(char) * 6, PROT_READ | PROT_WRITE,
                                   MAP_SHARED, fd, 0);
    shared_variable[0] = 'a';
    shared_variable[1] = 'b';
    shared_variable[2] = 'c';
    shared_variable[3] = 'd';
    shared_variable[4] = 'e';
    shared_variable[5] = 'f';
    // tutaj jedynie odczytujemy wartość pamięci - którą zmienia potencjalnie
    // inny proces, przykładowo python_and_c_mmap.py
    int i = 0;
    while (i < 50)
    {
        printf("%c %c %c %c %c %c \n", shared_variable[0], shared_variable[1], shared_variable[2],
               shared_variable[3], shared_variable[4], shared_variable[5]);
        sleep(1);
        i++;
    }
    // zwalniamy mapowanie
    munmap(shared_variable, 6);
    // zamykamy plik
    close(fd);
    return 0;
}
