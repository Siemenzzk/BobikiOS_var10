CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Isrc

SRC = src/global.c src/os.c src/resource.c src/task.c
TEST = tests/test_main.c

ifeq ($(OS), Windows_NT)
    TARGET = rtos.exe
    RM = del
else
    TARGET = rtos.out
    RM = rm -f
endif

all: $(TARGET)

$(TARGET): $(SRC) $(TEST)
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o $(TARGET)

clean:
	$(RM) $(TARGET)
