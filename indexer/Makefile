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

# both targets
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

# example valgrind test on letters depth 2
valgrind:
	make
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../crawler/test/letters/letters-2 ./test/letters/index.letters-2

# pipes test output to testing.out
test: 
	make
	./testing.sh &> testing.out

# phony targets (includes test directory)
.PHONY: safe test clean 

# clean directory; does not remove test data
clean: 
	rm -f $(EXECUTABLE1)
	rm -f $(EXECUTABLE2)
	rm -f *.o 
	$(CLEAN) -C $(LIBDIR)
	$(CLEAN) -C $(COMDIR)