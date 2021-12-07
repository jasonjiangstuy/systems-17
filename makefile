all: run.o setup.o
	gcc -o run run.o
	gcc -o setup setup.o

run.o: run.c
	gcc -c run.c

setup.o: setup.c
	gcc -c setup.c

clean:
	rm *.o
	rm *~
