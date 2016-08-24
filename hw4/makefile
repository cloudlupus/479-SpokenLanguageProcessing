CC = gcc
CFLAGS = -g -c -Wall

all: prog4

prog4: prog4.o
	$(CC) -Wall prog4.o -o prog4 -lm

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -f prog4.o prog4