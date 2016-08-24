CC = gcc
CFLAGS = -g -c -Wall

all: prog3

prog3: prog3.o
	$(CC) -Wall prog3.o -o prog3 -lm

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f prog3.o prog3