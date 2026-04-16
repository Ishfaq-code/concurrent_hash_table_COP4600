CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_XOPEN_SOURCE=700
LDFLAGS = -pthread

OBJS = chash.o database.o hash.o

all: chash

chash: $(OBJS)
	$(CC) $(CFLAGS) -o chash $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) chash

.PHONY: all clean
