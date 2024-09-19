all: myls

myls: main.o dirfn.o
	gcc main.o dirfn.o -o myls

main.o: main.c
	gcc -c main.c

dirfn.o: dirfn.c
	gcc -c dirfn.c

clean:
	rm -rf *.o myls
