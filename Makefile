build:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -lm -o game

release:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -o Pong

run:
	./game

clean:
	rm game