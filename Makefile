CC := gcc
CFLAGS += -Wall -Werror -pedantic

.PHONY: all clean
all: dijkstra

dijkstra: dijkstra.o parse.o
	$(CC) -o $@ $^

clean:
	$(RM) dijkstra.o parse.o
	$(RM) dijkstra

%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
