
# Konrad Pluta, nr albumu: 125152
### Informatyka II stopnia, grupa lab. 2
### Instrukcja laboratoryjna 1


## Zadanie 1 (`exec_example.c`)

### 1. Czy potrafisz wytłumaczyć, dlaczego nie zostanie wypisana linia z komunikatem: „I will never be printed!”?
Funkcja z rodziny `exec` (w tym przypadku `execvp`) zastępuje bieżący obraz procesu (kod, dane, stertę i stos) nowo uruchamianym programem (`wc`). Jeśli wywołanie `execvp` powiedzie się:
- Kod starego programu przestaje istnieć w pamięci tego procesu.
- Sterowanie przechodzi do punktu wejścia nowego programu.
- Instrukcje znajdujące się po wywołaniu `execvp` nigdy nie zostaną wykonane, chyba że samo wywołanie `execvp` zakończy się błędem (zwracając `-1`).

### 2. Zamień wywołanie funkcji `execvp` na `execv`, ale pozostaw argumenty bez zmian. Co się stało? Czy wiesz, jak można sprawić, aby ta wersja programu działała tak jak poprzednia?
- **Co się stało:** Program nie wykonał polecenia `wc`, lecz przeszedł do dalszych instrukcji i wypisał komunikat o błędzie / linii końcowej.
- **Dlaczego:** Litera `p` w nazwie `execvp` oznacza przeszukiwanie zmiennej środowiskowej `PATH` w celu odnalezienia pliku wykonywalnego. Funkcja `execv` nie przeszukuje ścieżek z `PATH` i oczekuje bezwzględnej (np. `/usr/bin/wc`) lub względnej ścieżki do pliku wykonywalnego. Przekazanie samego `"wc"` powoduje błąd `ENOENT` (*No such file or directory*).
- **Jak naprawić:** Należy podać pełną ścieżkę do programu jako pierwszy argument:
  ```c
  myargs[0] = strdup("/usr/bin/wc");
  execv(myargs[0], myargs);
  ```


## Zadanie 2 (`exec_error_handling.c`)

W tym programie wywołanie `execv("wc", myargs)` kończy się niepowodzeniem (kod `-1`), ponieważ nie podano pełnej ścieżki. Dzięki temu proces nie zostaje podmieniony, a program przechwytuje kod błędu ze zmiennej `errno`.
- Sprawdzenie `errsv == ENOENT` pozwala zidentyfikować błąd braku pliku/ścieżki i wypisać stosowny komunikat diagnostyczny.


## Zadanie 3 (`fork_hello_world_no_sync.c` oraz `fork_hello_world_sync.c`)

### 1. Jaki jest rezultat działania programu bez synchronizacji (`fork_hello_world_no_sync.c`)?
Proces rodzica wykonuje się szybciej niż proces potomka. Jest to najpewniej spowododwane tym, że rodzic już istnieje w pamięci, ma niezbędne zasoby, natomiast dziecko jest dopiero tworzone w systemie i procesor musi się podjąć jego wykonania. Do tego znak nowej linii od razu wypycha tekst wypisywany przez rodzica.

### 2. Synchronizacja (`fork_hello_world_sync.c`)
Aby zagwarantować poprawną kolejność (`Hello, World!`), proces rodzica musi zaczekać na zakończenie potomka za pomocą funkcji `wait(NULL)`. Dodatkowo w procesie potomnym warto użyć `fflush(stdout)`, aby wypchnąć dane z bufora przed zakończeniem:
```c
if (pid == 0) {
    printf("Hello, ");
    fflush(stdout);
} else {
    wait(NULL); // oczekiwanie na zakończenie potomka
    printf("World!\n");
}
```


## Zadanie 4 (`fork_variables.c`)

### 1. Jaki jest efekt działania powyższego programu?
Program wypisuje dwa wiersze:
```text
Parent process has variable a = abcbbb
Child process has variable a = aaadef
```

### 2. Czy potrafisz wyjaśnić dlaczego tak jest?
Podczas wywołania `fork()` system operacyjny tworzy kopię przestrzeni adresowej procesu rodzica dla procesu dziecka:
- Każdy proces posiada **własny, odizolowany segment danych i stos**.
- Zmiana zmiennej lokalnej `a` oraz globalnej `global_array` w procesie potomnym (indeksy 0..2 zmienione na `'a'`) modyfikuje wyłącznie pamięć procesu dziecka.
- Zmiana w procesie rodzica (indeksy 3..5 zmienione na `'b'`) modyfikuje wyłącznie pamięć procesu rodzica.
- Procesy nie dzielą wspólnej pamięci, dlatego zmiany dokonane w jednym procesie są niewidoczne w drugim.


## Zadanie 5 (`exec_and_fork.c`)

### Jaką kontrolę posiadamy nad procesem potomnym?
Proces rodzicielski posiada szeroką kontrolę nad procesem potomnym:
1. **Identyfikacja i synchronizacja:** Funkcja `fork()` zwraca procesowi rodzica identyfikator `PID` dziecka. Rodzic może oczekiwać na jego zakończenie za pomocą `wait(NULL)` lub `waitpid(pid, &status, options)` (także nieblokująco z flagą `WNOHANG`).
2. **Odczyt stanu zakończenia:** Poprzez makra (`WIFEXITED`, `WEXITSTATUS`, `WIFSIGNALED`, `WTERMSIG`) rodzic może sprawdzić, czy dziecko zakończyło się poprawnie, jaki zwróciło kod wyjścia lub jakim sygnałem zostało przerwane.
3. **Sterowanie sygnałami:** Rodzic może w każdej chwili wysłać sygnał do potomka za pomocą `kill(pid, SIG...)` (np. `SIGSTOP` – zatrzymanie, `SIGCONT` – wznowienie, `SIGTERM` / `SIGKILL` – wymuszenie zakończenia).
4. **Konfiguracja środowiska przed wykonaniem:** Przed wywołaniem `exec` w procesie potomnym rodzic może skonfigurować deskryptory wejścia/wyjścia (przekierowania potoków), katalog roboczy (`chdir`), zmienne środowiskowe oraz limity zasobów (`setrlimit`).


## Zadanie 6 (`pipe_example.c` oraz `python_pipe.c`)

### 1. Porównanie działania `pipe_example` z poleceniem w terminalu `ls | wc`:
Wynik działania programu w C oraz polecenia `ls | wc` jest **dokładnie taki sam**.
Powłoka (shell) pod maską realizuje potok w identyczny sposób jak program w C:
1. Tworzy potok za pomocą wywołania systemowego `pipe()`.
2. Tworzy dwa procesy potomne za pomocą `fork()`.
3. W pierwszym potomku przekierowuje standardowe wyjście `stdout` na wejście potoku (`dup2(p[1], STDOUT_FILENO)`) i uruchamia `ls`.
4. W drugim potomku przekierowuje standardowe wejście `stdin` z wyjścia potoku (`dup2(p[0], STDIN_FILENO)`) i uruchamia `wc`.
5. Zamyka nieużywane końce potoku i czeka na zakończenie procesów za pomocą `wait()`.

### 2. Program z procesem Pythona (`python_pipe.c`):
Program realizuje wymagania:
- Tworzy proces potomny, który wykonuje:
  ```bash
  python3 -c 'for i in range(10): print(i)'
  ```
  ze standardowym wyjściem przekierowanym do potoku `pipe`.
- Proces rodzicielski odczytuje surowe dane z potoku do bufora w pętli `read()`.
- Odczytane liczby są parsowane, a program weryfikuje ich parzystość (`num % 2 == 0`).
- Na koniec wypisywane jest podsumowanie: łączna liczba odczytanych liczb (10) oraz liczba liczb parzystych (5: 0, 2, 4, 6, 8).
