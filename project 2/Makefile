all: ass1

ass1: memgrind.o mymalloc.o
	gcc -g -o ass1 mymalloc.c memgrind.c

mymalloc: mymalloc.o
	gcc -g -c mymalloc.c

memgrind: memgrind.c
	gcc -g -c memgrind.c

clean:
	rm -f *.o ass1