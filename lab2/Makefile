CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = shared_memory shared_memory_wait shared_memory_hello_world mmap python_and_c_mmap

all: $(TARGETS)

shared_memory: shared_memory.c
	$(CC) $(CFLAGS) -o $@ $<

shared_memory_wait: shared_memory_wait.c
	$(CC) $(CFLAGS) -o $@ $<

shared_memory_hello_world: shared_memory_hello_world.c
	$(CC) $(CFLAGS) -o $@ $<

mmap: mmap.c
	$(CC) $(CFLAGS) -o $@ $<

python_and_c_mmap: python_and_c_mmap.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) temp_mmap.txt

.PHONY: all clean
