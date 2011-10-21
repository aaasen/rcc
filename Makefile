CFLAGS = -Wall
FILES = main.c rtree.h
CC = gcc
OUTPUT = rtree

make:
	$(CC) $(CFLAGS) $(FILES) -o $(OUTPUT)