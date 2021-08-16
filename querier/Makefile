# Makefile for querier module
FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
LIBDIR = ../libcs50
COMDIR =../common

querier: querier.o ../common/common.a ../libcs50/libcs50.a
	$(CC) $(FLAGS) -o querier querier.c ../common/common.a ../libcs50/libcs50.a

unittest: querier.o ../common/common.a ../libcs50/libcs50.a
	$(CC) $(FLAGS) -o querier querier.c ../common/common.a ../libcs50/libcs50.a -DUNITTEST
	./querier

test: 
	./testing.sh

# library dependencies
../libcs50/libcs50.a: 
	make -C $(LIBDIR)

../common/common.a:
	make -C $(COMDIR)

.PHONY: safe test unittest clean 

clean: 
	rm *.o
	rm -f querier