all: snake

snake: snake.o
	gcc snake.o -o snake -lncurses

snake.o: snake.c
	gcc -c snake.c -o snake.o

clean:
	rm -f *.o snake
