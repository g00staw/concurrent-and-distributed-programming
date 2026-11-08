# Laboratorium 3 — skrót

> Uruchamiaj w Linuxie albo WSL, nie w zwykłym PowerShellu. W WSL doinstaluj:
> `sudo apt install build-essential libncurses-dev`

## Kompilacja

W katalogu `lab3` wykonaj `make`. Każdy program uruchamiasz przez `./nazwa_programu`.
Po awarii można sprawdzić zasoby IPC przez `ipcs`; własną kolejkę usuń przez
`ipcrm -Q 1234`, a pamięć współdzieloną przez `ipcrm -m <shmid>`.

## 1. Sygnały i semafor

Pliki: `signals_hello_world.c`, `signals_hello_world_sleep.c`,
`signals_hello_world_semaphore.c`.

1. Uruchom każdą wersję kilka razy.
2. Pierwsza wersja ma **race condition**: rodzic może wysłać `SIGUSR1`, zanim
   dziecko zainstaluje handler, więc `World!` czasem się nie pojawi.
3. `usleep(10)` tylko zmniejsza prawdopodobieństwo błędu — nigdy nie daje
   gwarancji, bo planista systemu nie gwarantuje czasu wykonania procesu.
4. Semafor o wartości początkowej `0` tworzy poprawne potwierdzenie gotowości:
   dziecko wykonuje `sem_post`, a rodzic czeka w `sem_wait`.
5. Zmień wartość początkową semafora na `1` i obserwuj powrót błędu wyścigu.
   Po `wait` dodaj `sleep(10)` i sprawdź `/dev/shm`: semafor istnieje do
   `sem_unlink`.

`wait()` nie zastępuje tu semafora: rodzic może nim czekać na **zakończenie**
dziecka, ale dziecko nie ma procesu potomnego i nie może czekać na gotowość rodzica.

## 2. Kolejka komunikatów System V

Plik: `message_queue_hello_world.c`.

- Uruchom kilka razy: `msgrcv` blokuje dziecko, aż rodzic wyśle wiadomość,
  dlatego kolejność `Hello world!` jest zachowana.
- Zakomentuj `msgsnd`: dziecko będzie czekać w `msgrcv`.
- Ustaw flagę `IPC_NOWAIT` w `msgrcv`: zamiast czekać dostaniesz błąd
  `ENOMSG`, gdy kolejka jest pusta.
- Własne ćwiczenie: użyj dwóch typów wiadomości — rodzic wysyła `Hello`
  (typ 1), dziecko odbiera je i odsyła `World!` (typ 2).

## 3. Chat blokujący — pamięć współdzielona i sygnały

Pliki: `chat1.c`, `chat2.c`.

W dwóch terminalach uruchom kolejno `./chat1`, potem `./chat2`. Programy używają
jednego bufora, więc rozmowa jest naprzemienna: User2 zaczyna, potem odpowiada
User1. Zakończ wpisując `q` w pustej linii.

Zadanie: usuń oczekiwanie na swoją kolej (pętle/pauzy) i oceń efekt. Jeden
bufor jest ryzykowny: równoczesny zapis może nadpisać nieodczytaną wiadomość.

## 4. Chat nieblokujący

Pliki: `chat_1_non_blocking.c`, `chat_2_non_blocking.c`,
`chat_non_blocking_common.h`.

Uruchom kolejno oba programy. `ncurses` ustawia wejście nieblokujące: użytkownik
może dalej pisać, kiedy druga strona wyśle wiadomość. Są dwa osobne bufory,
lokalny bufor tekstu i obsługa Backspace. Handler sygnału tylko ustawia flagę;
rysowanie terminala odbywa się w pętli głównej — wywoływanie `ncurses` bezpośrednio
z handlera byłoby niebezpieczne.

Scenariusz do sprawdzenia: zacznij pisać tekst w jednym terminalu bez Enter,
wyślij wiadomość z drugiego. Tekst w trakcie pisania powinien pozostać widoczny.

## 5. Named pipe (FIFO)

Pliki: `named_pipe_1.c`, `named_pipe_2.c`.

Uruchom `./named_pipe_1`, a w drugim terminalu `./named_pipe_2` (kolejność nie
jest istotna — otwarcie FIFO do zapisu/odczytu blokuje do momentu połączenia
drugiej strony).

FIFO ma nazwę w systemie plików i może łączyć niezależnie uruchomione procesy.
Zwykły `pipe()` jest anonimowy, nie ma ścieżki i najczęściej służy procesom
spokrewnionym po `fork()`.

## Zadania dodatkowe

6. Zastąp dwa bufory czatu dwiema FIFO (po jednej w każdym kierunku), najpierw
wersja blokująca, potem nieblokująca.
7. Napisz chat na kolejce komunikatów: wiadomości User1→User2 mają typ `1`,
a User2→User1 typ `2`; dodaj sygnały i nieblokujące odbieranie.
