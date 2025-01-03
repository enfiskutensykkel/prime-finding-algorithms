CC := gcc
CFLAGS += -Wall -Wextra -pedantic -DNDEBUG

.PHONY: all clean
all: dijkstra eratosthenes trial

dijkstra: dijkstra.o parse.o
	$(CC) -o $@ $^

eratosthenes: eratosthenes.o parse.o isqrt.o
	$(CC) -o $@ $^

trial: trial.o parse.o isqrt.o
	$(CC) -o $@ $^

clean:
	-$(RM) parse.o isqrt.o
	-$(RM) eratosthenes eratosthenes.o
	-$(RM) dijkstra dijkstra.o
	-$(RM) trial trial.o

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
