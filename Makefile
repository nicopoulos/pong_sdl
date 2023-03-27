build:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lm -O0 -g3 -o game

release:
	gcc -Wall -std=c17 ./source/*.c -lSDL2 -lSDL2_ttf -lSDL2_image -lm -O3 -o Pong

run:
	./game

debug:



clean:
	rm game