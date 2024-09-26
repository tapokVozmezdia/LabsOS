all: myls

myls: main.o dirfn.o chsort.o
	gcc main.o dirfn.o chsort.o -lm -o myls

main.o: main.c
	gcc -g -c main.c

dirfn.o: dirfn.c
	gcc -g -c dirfn.c

chsort.o: chsort.c
	gcc -g -c chsort.c

clean:
	rm -rf *.o myls
