CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Isrc

SRC = src/global.c src/os.c src/resource.c src/task.c
TEST = tests/test_main.c

all: rtos

rtos: $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o rtos.exe

clean:
	del rtos.exe
