# Make file by Dylan Swiggett
# Written on Linux Mint 11
# Only compatible with unix/linux based operating systems
#
# make objects - compile all c files to objects
# make exe - compile all objects to an executable
# make clean - remove all object files
# make all - compile objects, make executable, clean directory
# make run - do full compile and clean, then run
# make leak - make all and then run a full valgrind leak check on the output

CFLAGS = -Wall -lm
FILES = main.c rtree.c point.c rect.c qdbmp.c hmap.c parray.c\
	 sphere.c cylinder.c rectarray.c
OBJECTS = main.o rtree.o point.o rect.o qdbmp.o hmap.o parray.o\
	 sphere.o cylinder.o rectarray.o
MAIN = main
CC = gcc
OUTPUT = rtree

objects:
	$(CC) -g -c $(FILES)			# -g for debugging

exe:
	$(CC) -g $(CFLAGS) $(OBJECTS) -o $(OUTPUT)

clean:
	rm $(OBJECTS)

all:
	make objects
	make exe
	make clean
	@echo Yay! Compiled

run:
	make all
	./$(OUTPUT)

leak:
	make all
	valgrind --leak-check=full ./$(OUTPUT)
