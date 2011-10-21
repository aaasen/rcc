CFLAGS = -Wall -lm
FILES = main.c rtree.c
MAIN = main
OBJECTS = main.o rtree.o
CC = gcc
OUTPUT = rtree

objects:
	$(CC) -c $(FILES)

exe:
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT)

clean:
	rm $(OBJECTS)

all:
	make objects
	make exe
	make clean
