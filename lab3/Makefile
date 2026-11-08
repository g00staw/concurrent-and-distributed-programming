CC := gcc
CFLAGS := -D_DEFAULT_SOURCE -Wall -Wextra -Wpedantic -std=c11

PROGRAMS := signals_hello_world signals_hello_world_sleep \
	signals_hello_world_semaphore message_queue_hello_world \
	chat1 chat2 chat_1_non_blocking chat_2_non_blocking \
	named_pipe_1 named_pipe_2

all: $(PROGRAMS)

signals_hello_world: signals_hello_world.c
	$(CC) $(CFLAGS) $< -o $@

signals_hello_world_sleep: signals_hello_world_sleep.c
	$(CC) $(CFLAGS) $< -o $@

signals_hello_world_semaphore: signals_hello_world_semaphore.c
	$(CC) $(CFLAGS) $< -o $@ -pthread

message_queue_hello_world: message_queue_hello_world.c
	$(CC) $(CFLAGS) $< -o $@

chat1: chat1.c
	$(CC) $(CFLAGS) $< -o $@

chat2: chat2.c
	$(CC) $(CFLAGS) $< -o $@

chat_1_non_blocking: chat_1_non_blocking.c chat_non_blocking_common.h
	$(CC) $(CFLAGS) chat_1_non_blocking.c -o $@ -lncurses

chat_2_non_blocking: chat_2_non_blocking.c chat_non_blocking_common.h
	$(CC) $(CFLAGS) chat_2_non_blocking.c -o $@ -lncurses

named_pipe_1: named_pipe_1.c
	$(CC) $(CFLAGS) $< -o $@

named_pipe_2: named_pipe_2.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(PROGRAMS)

.PHONY: all clean
