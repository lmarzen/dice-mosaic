dice-mosaic: main.c
	gcc main.c -o dice-mosaic -Wall -Werror -O3 -lm

clean:
	rm -f *- dice-mosaic core
