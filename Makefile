

.PHONY: all

all: argp-test


argp-test: argp.c main.c
	$(CC) argp.c main.c -o argp-test  