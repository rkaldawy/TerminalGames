all: snake

snake: snake.o
	gcc snake.o -o snake -lncurses

snake.o: hello_world.c
	gcc -c hello_world.c -o snake.o

clean:
	rm -f *.o snake
