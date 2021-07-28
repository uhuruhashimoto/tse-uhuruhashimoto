# Makefile for common library
# builds object files from common.c

CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIB = common.a
OBJS = pagedir.o

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# dependencies 
pagedir.o: pagedir.h

.PHONY: clean

clean: 
	rm -f *.o
	rm -f $(LIB)
