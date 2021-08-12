FLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
LIBDIR = ../libcs50
COMDIR =../common

querier: querier.o ../common/common.a ../libcs50/libcs50.a
	$(CC) $(FLAGS) -o querier querier.c ../common/common.a ../libcs50/libcs50.a

unittest: querier.o ../common/common.a ../libcs50/libcs50.a
	$(CC) $(FLAGS) -o querier querier.c ../common/common.a ../libcs50/libcs50.a -DUNITTEST
	
# library dependencies
../libcs50/libcs50.a: 
	make -C $(LIBDIR)

../common/common.a:
	make -C $(COMDIR)

clean: 
	rm *.o
	rm -f querier