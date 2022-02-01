dice-mosaic: main.c
	gcc main.c -o dice-mosaic -Wall -O3 -lm
	x86_64-w64-mingw32-gcc -o dice-mosaic.exe main.c -Wall -O3
linux: main.c
	gcc main.c -o dice-mosaic -Wall -O3 -lm
windows: main.c
	x86_64-w64-mingw32-gcc -o dice-mosaic.exe main.c -Wall -O3
unoptimized: main.c
	gcc main.c -o dice-mosaic -Wall -lm
clean:
	rm -f *- dice-mosaic dice-mosaic.exe core
