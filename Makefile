CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude

SRC = src/main.c src/repl.c
OBJ = $(SRC:.c=.o)

.PHONY: all test run clean

all: mathlang

mathlang: $(OBJ)
	$(CC) $(CFLAGS) -o mathlang $(OBJ)

run: mathlang
	./mathlang

clean:
	rm -f $(OBJ) mathlang