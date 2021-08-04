# Makefile for indexer module

OBJS = crawler.o
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
CLEAN = make clean
LIBDIR = ../libcs50
COMDIR = ../common
LIB = ../common/common.a ../libcs50/libcs50.a
EXECUTABLE1 = indexer
EXECUTABLE2 = indextest
CFILE1 = indexer.o
CFILE2 = indextest.o

all: indexer indextest

$(EXECUTABLE1): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE1) $(CFILE1) $(LIB)

$(EXECUTABLE2): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE2) $(CFILE2) $(LIB)

# library dependencies
../libcs50/libcs50.a: 
	$(MAKE) -C $(LIBDIR)

../common/common.a:
	$(MAKE) -C $(COMDIR)

# driver dependencies
indexer.o: indexer.h

indextest.o: indextest.h

#no overwrites
safe: ../libcs50/libcs50.a
	make -C $(COMDIR) safe
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(CFILE) $(LIB)

test: 
	make
	./testing.sh &> testing.out

.PHONY: safe test clean 

clean: 
	rm -f $(EXECUTABLE)
	rm -f *.o 
	$(CLEAN) -C $(LIBDIR)
	$(CLEAN) -C $(COMDIR)