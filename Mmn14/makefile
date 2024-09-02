.NOTPARALLEL:
MAKEFLAGS += --no-builtin-rules --no-builtin-variables

CC = gcc
CFLAGS = -ansi -Wall -g -pedantic
SRC_DIR = src
HEADER_DIR = header
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)
EXEC = assembler

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

clean:
	rm -f $(SRC_DIR)/*.o $(EXEC)

.PHONY: all clean
