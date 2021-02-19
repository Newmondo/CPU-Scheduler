all: idispatcher
idispatcher: idispatcher.o
	gcc -o idispatcher idispatcher.o -lfl
idispatcher.o: idispatcher.c
	gcc -Wall -std=c99 -g -c idispatcher.c
clean:
	rm -f *.o
	rm -f idispatcher