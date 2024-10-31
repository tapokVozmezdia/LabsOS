all: mychmod

mychmod: main.o chmfn.o
	gcc main.o chmfn.o -o mychmod

main.o: main.c
	gcc -g -c main.c

chmfn.o: chmfn.c
	gcc -g -c chmfn.c

clean:
	rm -rf *.o chmfn