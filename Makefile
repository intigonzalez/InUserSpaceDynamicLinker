CC=gcc
CFLAGS=-m32 -std=gnu99

SRC=src/main.c src/FileManagement.c src/Allocator.c src/elfloader.c src/MyHashTable.c
OBJS=$(SRC:.c=.o)
EXECUTABLE=loader

all: $(SRC) $(EXECUTABLE)
	cd example && $(MAKE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	rm -f $(EXECUTABLE)
	rm -f $(OBJS)
	cd example && $(MAKE) $@

run: all
	@echo ""	
	@echo "###############################################"
	@echo "#                                             #"
	@echo "#             Read the README file            #"
	@echo "#                                             #"
	@echo "###############################################"
	./$(EXECUTABLE) example/factorial.o
