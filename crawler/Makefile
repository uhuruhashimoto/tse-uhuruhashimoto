# Makefile for crawler module

OBJS = crawler.o
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
CLEAN = make clean
LIBDIR = ../libcs50
COMDIR = ../common
LIB = ../common/common.a ../libcs50/libcs50.a
EXECUTABLE = crawler
CFILE = crawler.c

$(EXECUTABLE): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(CFILE) $(LIB)

# library dependencies
../libcs50/libcs50.a: 
	$(MAKE) -C $(LIBDIR)

../common/common.a:
	$(MAKE) -C $(COMDIR)

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
