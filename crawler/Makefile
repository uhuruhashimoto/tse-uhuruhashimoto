# Makefile for crawler module

OBJS = crawler.o
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make
CLEAN = make clean
LIBDIR = ../common ../libcs50
LIB = ../common/common.a ../libcs50/libcs50.a
EXECUTABLE = crawler
CFILE = crawler.c

$(EXECUTABLE): $(OBJS) $(LIB)
	$(CC) $(FLAGS) -o $(EXECUTABLE) $(CFILE) $(LIB)

# library dependency
$(LIB): 
	$(MAKE) -C $(LIBDIR)

clean: 
	rm -f $(EXECUTABLE)
	rm -f *.o 
	$(CLEAN) -C $(LIBDIR)
