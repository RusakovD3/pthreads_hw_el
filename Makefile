CC=gcc
CFLAGS=-pthread -Wall -Werror -Wextra
TARGETS=task_1 task_2 task_3

.PHONY: all clean

all: $(TARGETS)

task_1: task_1.c
	$(CC) $(CFLAGS) -o task_1 task_1.c

task_2: task_2.c
	$(CC) $(CFLAGS) -o task_2 task_2.c

task_3: task_3.c
	$(CC) $(CFLAGS) -o task_3 task_3.c

clean:
	rm -f $(TARGETS)
