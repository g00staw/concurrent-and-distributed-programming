CC = gcc
CFLAGS = -Wall -Wextra

TARGETS = exec_example exec_error_handling fork_hello_world_no_sync fork_hello_world_sync fork_variables exec_and_fork pipe_example python_pipe

all: $(TARGETS)

exec_example: exec_example.c
	$(CC) $(CFLAGS) -o $@ $<

exec_error_handling: exec_error_handling.c
	$(CC) $(CFLAGS) -o $@ $<

fork_hello_world_no_sync: fork_hello_world_no_sync.c
	$(CC) $(CFLAGS) -o $@ $<

fork_hello_world_sync: fork_hello_world_sync.c
	$(CC) $(CFLAGS) -o $@ $<

fork_variables: fork_variables.c
	$(CC) $(CFLAGS) -o $@ $<

exec_and_fork: exec_and_fork.c
	$(CC) $(CFLAGS) -o $@ $<

pipe_example: pipe_example.c
	$(CC) $(CFLAGS) -o $@ $<

python_pipe: python_pipe.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
	rm -rf CRUDDemoFront

.PHONY: all clean
