dice-mosaic: main.c
	gcc main.c -o dice-mosaic -Wall -O3 -lm
	i686-w64-mingw32-gcc -o dice-mosaic_x86.exe main.c -Wall -O3
	x86_64-w64-mingw32-gcc -o dice-mosaic_x64.exe main.c -Wall -O3
linux: main.c
	gcc main.c -o dice-mosaic -Wall -O3 -lm
win32: main.c
	i686-w64-mingw32-gcc -o dice-mosaic_x86.exe main.c -Wall -O3
win64: main.c
	x86_64-w64-mingw32-gcc -o dice-mosaic_x64.exe main.c -Wall -O3
unoptimized: main.c
	gcc main.c -o dice-mosaic -Wall -lm
debug: main.c
	gcc main.c -o dice-mosaic -Wall -lm -g
clean:
	rm -f *- dice-mosaic dice-mosaic_x86.exe dice-mosaic_x64.exe core
