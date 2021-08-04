# Makefile for indexer module


FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
CLEAN = make clean
LIBDIR = ../libcs50
COMDIR = ../common
LIB = ../common/common.a ../libcs50/libcs50.a
EXECUTABLE1 = indexer
EXECUTABLE2 = indextest
CFILE1 = indexer.c
CFILE2 = indextest.c

all: indexer indextest

$(EXECUTABLE1): indexer.o $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE1) $(CFILE1) $(LIB)

$(EXECUTABLE2): indextest.o $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE2) $(CFILE2) $(LIB)

# library dependencies
../libcs50/libcs50.a: 
	$(MAKE) -C $(LIBDIR)

../common/common.a:
	$(MAKE) -C $(COMDIR)


#no overwrites
safe: ../libcs50/libcs50.a
	make -C $(COMDIR) safe
	make all

test: 
	make
	./testing.sh &> testing.out

.PHONY: safe test clean 

clean: 
	rm -f $(EXECUTABLE1)
	rm -f $(EXECUTABLE2)
	rm -f *.o 
	$(CLEAN) -C $(LIBDIR)
	$(CLEAN) -C $(COMDIR)