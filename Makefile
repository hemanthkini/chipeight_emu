all: main.c graphics.c core.c input.c
	clang graphics.c core.c input.c main.c -o chip8 -I/usr/local/include -L/usr/local/lib -lSDL2

clean:
	rm -f ./chip8; rm -f *~
