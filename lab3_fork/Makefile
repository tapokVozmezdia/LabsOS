all: fork

fork: main.o sgfn.o
	gcc main.o sgfn.o -lm -o fork

main.o: main.c
	gcc -g -c main.c

sgfn.o: sgfn.c
	gcc -g -c sgfn.c

clean:
	rm -rf *.o fork