# Makefile for crawler module

OBJS = crawler.o
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
CLEAN = make clean
LIB = ../libcs50/libcs50.a
EXECUTABLE = crawler
CFILE = crawler.c

$(EXECUTABLE): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(CFILE) $(LIB)

# library dependency
$(LIB): 
	$(MAKE) -C ../libcs50

clean: 
	rm -f $(EXECUTABLE)
	rm -f *.o 
	$(CLEAN) -C ../libcs50
