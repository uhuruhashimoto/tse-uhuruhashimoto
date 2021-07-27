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
bag.o: bag.h
counters.o: counters.h
file.o: file.h
hashtable.o: hashtable.h
jhash.o: jhash.h
memory.o: memory.h
set.o: set.h
webpage.o: webpage.h

clean: 
	rm -f $(EXECUTABLE)
	rm -f *.o 