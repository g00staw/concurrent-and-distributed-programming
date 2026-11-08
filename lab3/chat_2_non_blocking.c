#include "chat_non_blocking_common.h"

#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

static volatile sig_atomic_t has_message = 0;

static void handler(int signum) {
    if (signum == SIGUSR2)
        has_message = 1;
}

int main(void) {
    int shmid = shmget(CHAT_KEY, sizeof(struct chat_memory), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        return EXIT_FAILURE;
    }
    struct chat_memory *shared = shmat(shmid, NULL, 0);
    if (shared == (void *)-1) {
        perror("shmat");
        return EXIT_FAILURE;
    }

    shared->pid2 = getpid();
    signal(SIGUSR2, handler);

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    printw("User2: ");
    refresh();

    char local[CHAT_CAPACITY] = "";
    size_t length = 0;
    for (;;) {
        if (has_message) {
            has_message = 0;
            printw("\nReceived from User1: %sUser2: %s", shared->from_user1, local);
            refresh();
        }

        int key = getch();
        if (key == ERR) {
            usleep(10 * 1000);
            continue;
        }
        if (key == 27 || (key == 'q' && length == 0))
            break;
        if ((key == KEY_BACKSPACE || key == 127 || key == '\b') && length > 0) {
            local[--length] = '\0';
            printw("\b \b");
        } else if (key == '\n' || key == KEY_ENTER) {
            local[length++] = '\n';
            local[length] = '\0';
            while (shared->pid1 == 0)
                usleep(1000);
            strcpy(shared->from_user2, local);
            kill(shared->pid1, SIGUSR1);
            length = 0;
            local[0] = '\0';
            printw("\nUser2: ");
        } else if (key >= 32 && key < 127 && length < CHAT_CAPACITY - 2) {
            local[length++] = (char)key;
            local[length] = '\0';
            addch(key);
        }
        refresh();
    }

    endwin();
    shmdt(shared);
    return EXIT_SUCCESS;
}
