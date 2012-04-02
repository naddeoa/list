

default: all

all:
	gcc -o demo demo.c list.h list.c

clean:
	rm *.o
