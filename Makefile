build:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lm -O0 -g -o game

run:
	./game

debug:
	gdb ./game

release:
	gcc -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lm -O3 -o Pong




debug:



clean:
	rm game