# Konrad Pluta, nr albumu: 125152
### Informatyka II stopnia, grupa lab. 2
### Instrukcja laboratoryjna 2


## Zadanie 1 (`shared_memory.c`, `shared_memory_wait.c`, `shared_memory_hello_world.c`)

### 1. Usuń w procesie rodzica instrukcje wait i zapisz nowy plik jako shared_memory_wait.c. Jaki jest efekt działania programu?
Usunięcie `wait(NULL)` wywołuje wyścig (race condition) i brak synchronizacji:
- Rodzic nie czeka na potomka, czyta pusty bufor, dopisuje `bbb` i usuwa segment (`IPC_RMID`).
- Potomek startuje później, zeruje bufor i nadpisuje pamięć ciągiem `aaa`.
- Kolejność wykonania i zawartość pamięci są niedeterministyczne.

Wynik z terminala (`./shared_memory_wait`):
```text
Parent reads 
Parent append bbb

Child append aaa
```

### 2. Zmodyfikuj powyższy program, aby na ekranie został wypisany Hello World! Nazwij plik shared_memory_hello_world.c
Potomek wpisuje do pamięci współdzielonej `"Hello"`, a rodzic po odczekaniu (`wait(NULL)`) odczytuje bufor i dopisuje `" World!"`.

Wynik z terminala (`./shared_memory_hello_world`):
```text
Child append Hello
Parent reads Hello
Parent append Hello World!
```


## Zadanie 2 (`mmap.c`)

### 1. Jaki jest efekt działania programu?
- **Zmienna lokalna `a`:** nie jest współdzielona. Po `fork()` każdy proces ma własną kopię pamięci. Potomek zmienia indeksy 0–2 na `'a'` (`aaadef`), a rodzic zmienia indeksy 3–5 na `'b'` (`abcbbb`) – zmiany się nie przenikają.
- **Zmienna `shared_variable`:** jest współdzielona w pamięci fizycznej dzięki `mmap`. Rodzic widzi modyfikację potomka (`aaa`) i dopisuje swoje litery (`bbb`), uzyskując ostatecznie `aaabbb`.

Wynik z terminala (`./mmap`):
```text
Child process has variable a = aaadef
Child process has shared variable = aaadef
Shared variable outside process block = aaadef
Parent process has variable a = abcbbb
Parent process has shared variable = aaabbb
Shared variable outside process block = aaabbb
```

### 2. Co dało użycie funkcji mmap?
- Współdzielenie pamięci bezpośrednio w RAM bez tworzenia pliku na dysku (`MAP_ANONYMOUS | MAP_SHARED`).
- Komunikację bez kopiowania danych przez bufor jądra (zero-copy) i bez wywołań systemowych przy transferze danych.
- Prostsze zarządzanie niż w System V IPC (zwraca wskaźnik, brak konieczności operowania na kluczach/identyfikatorach, zwolnienie przez `munmap`).


## Zadanie 3 (`python_and_c_mmap.c`, `python_and_c_mmap.py`)

### 1. Czym różni się użycie mmap od sytuacji, w której po prostu otworzono by plik i zapisywano do niego dane?
- **Wydajność (Zero-copy):** operacje bezpośrednio w RAM na wskaźnikach zamiast narzutu wywołań systemowych `read`/`write` i kopiowania buforów jądra.
- **Komunikacja w czasie rzeczywistym:** modyfikacja pamięci przez Pythona jest natychmiast widoczna w C bez konieczności ciągłego opróżniania buforów (`flush`) i ponownego odczytu z pozycjonowaniem kursora (`seek`/`read`).
- **Dostęp swobodny:** plik jest indeksowany bezpośrednio jak zwykła tablica bajtów.

Wynik z terminala:

Terminal 1 (`python3 python_and_c_mmap.py`):
```text
b'abcdef'
b'abcdef'
b'abcxef'
b'axcxef'
b'axxxef'
b'xxxxef'
```

Terminal 2 (`./python_and_c_mmap`):
```text
a b c d e f 
a b c x e f 
a x c x e f 
a x x x e f 
x x x x e f 
x x x x e f 
```

### 2. Czy widać jakieś ograniczenie w powyższym mechanizmie?
- **Brak wbudowanej synchronizacji:** ryzyko wyścigu danych przy jednoczesnym zapisie/odczycie (wymaga zewnętrznych semaforów/blokad).
- **Sztywny rozmiar pliku:** `mmap` nie rozszerza pliku; próba odwołania poza rozmiar kończy się zrzutem pamięci i błędem magistrali (`SIGBUS`).
- **Zgodność formatu:** konieczność ręcznego dbania o zgodność typów binarnych, kolejność bajtów (endianness) i wyrównanie struktur między C a Pythonem.
- **Asynchroniczna trwałość:** modyfikacje trafiają do cache stron; zapis na fizyczny dysk wymaga wywołania `msync`.