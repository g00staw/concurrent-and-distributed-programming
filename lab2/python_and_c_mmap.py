import mmap
import os
import random
import time

fname = './temp_mmap.txt'
# jeżeli plik jeszcze nie istnieje, tworzymy go
# i inicjujemy jakąś wartością
if not os.path.isfile(fname):
    with open(fname, "wb+") as fd:
        fd.write(b'abcdef')

with open(fname, "rb+") as fd:
    # mapujemy zawartość pliku, określamy że ma on 6 bajtów, ustawiamy prawo zapisu do pamięci współdzielonej, na koniec
    # określamy że mapujemy cały plik - offset 0
    mm = mmap.mmap(fd.fileno(), 6, access=mmap.ACCESS_WRITE, offset=0)
    # wypisujemy pierwsze 6 bajtów
    print(mm.read(6))
    # tutaj w celach pokazowych
    # program losowo podmienia 5 razy litery na x
    counter = 0
    while True:
        index = random.randint(0, 5)
        mm[index] = 120 # kod znaku x w ascii
        time.sleep(1)
        mm.seek(0) # przesuwamy się na pozycję 0 w buforze
        print(mm.readline()) # odczytujemy pełną linię
        counter += 1
        if counter == 5:
            break
    # zamykamy strumienie
    mm.close()
    fd.close()
