# Makefile for common library
# builds object files from common.c

CFLAGS = -Wall -pedantic -std=c11 -ggdb
LIB = common.a
OBJS = pagedir.o index.o word.o

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# no overwrites
safe: 
	gcc $(CFLAGS) -c pagedir.c -DNOOVERWRITE
	ar cr $(LIB) $(OBJS)

# dependencies 
pagedir.o: pagedir.h
index.o: index.h
word.o: word.h

.PHONY: clean

clean: 
	rm -f *.o
	rm -f $(LIB)
