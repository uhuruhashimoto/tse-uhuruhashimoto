# Makefile for crawler module

OBJS = bag.o counters.o file.o hashtable.o jhash.o memory.o set.o webpage.o
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
LIB = ../libcs50/libcs50.a
EXECUTABLE = crawler
CFILE = crawler.c

$(EXECUTABLE): $(OBJS)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(CFILE) -L $(LIB)

# dependencies (headers)


clean: 
	rm -f $(EXECUTABLE)
	rm -f *.o 