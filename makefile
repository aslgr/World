CFLAGS = -Wall -g
LDFLAGS = -lm

CC = gcc

objects = Mundo.o LibFila.o LibConjunto.o LibLEF.o

all: Mundo

Mundo: $(objects)
	$(CC) $^ -o $@ $(LDFLAGS)

Mundo.o: Mundo.c LibFila.h LibConjunto.h LibLEF.h
	$(CC) -c $< $(CFLAGS)

LibFila.o: LibFila.c
	$(CC) -c $< $(CFLAGS)

LibConjunto.o: LibConjunto.c
	$(CC) -c $< $(CFLAGS)

LibLEF.o: LibLEF.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f $(objects)

purge:
	rm -f $(objects) Mundo
