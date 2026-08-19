# Compiler and flags
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g -Iinclude 
# Implicit rules: Make knows how to compile .c → .o. Since CFLAGS includes -Iinclude, headers are found.

# Library sources (all .c except main.c)
LIB_SRC = src/repl.c src/lexer.c
LIB_OBJ = $(LIB_SRC:.c=.o)

# Main program sources (only main.c)
MAIN_SRC = src/main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

# Test sources (all .c files in tests/)
TEST_SRC = $(wildcard tests/*.c)
TEST_OBJ = $(TEST_SRC:.c=.o)

# Binary names
MAIN_BIN = mathlang
TEST_BIN = test_runner

# Phony targets
.PHONY: all test run clean

# Default target
all: $(MAIN_BIN)

# Build main executable
$(MAIN_BIN): $(LIB_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) -o $(MAIN_BIN) $(LIB_OBJ) $(MAIN_OBJ)

# Build and run tests
test: $(TEST_BIN)
	./$(TEST_BIN)

# Build test binary (link Criterion)
$(TEST_BIN): $(LIB_OBJ) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $(TEST_BIN) $(LIB_OBJ) $(TEST_OBJ) -lcriterion

# Run main program
run: $(MAIN_BIN)
	./$(MAIN_BIN)

# Clean build artifacts
clean:
	rm -f $(LIB_OBJ) $(MAIN_OBJ) $(TEST_OBJ) $(MAIN_BIN) $(TEST_BIN)