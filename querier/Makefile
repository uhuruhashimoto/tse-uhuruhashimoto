FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
LIBDIR = ../libcs50
COMDIR =../common

querier: querier.o ../libcs50/libcs50.a ../common/common.a
	$(CC) $(FLAGS) -o $@ querier.c ../libcs50/libcs50.a ../common/common.a
	
# library dependencies
../libcs50/libcs50.a: 
	$(MAKE) -C $(LIBDIR)

../common/common.a:
	$(MAKE) -C $(COMDIR)

clean: 
	rm *.o
	rm -f querier