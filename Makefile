build:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm -O0 -g -o game

run:
	./game

debug:
	gdb ./game

release:
	gcc -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lm -O3 -o Pong




debug:



clean:
	rm game