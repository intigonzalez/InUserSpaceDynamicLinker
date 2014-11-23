
CC=gcc
CFLAGS=-m32 -std=gnu99 -fno-asynchronous-unwind-tables

SRC=factorial.c
OBJS=$(SRC:.c=.o)

all: $(OBJS)
	
%.o:%.c 
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS)

