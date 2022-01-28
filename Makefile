dice-mosaic: main.c
	gcc main.c -o dice-mosaic -Wall -Werror -O3 -lpng

clean:
	rm -f *- dice-mosaic core
