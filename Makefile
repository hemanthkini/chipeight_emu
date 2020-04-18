all: main.c graphics.c core.c
	clang graphics.c core.c main.c -o chip8 -framework SDL2

clean:
	rm -f ./chip8; rm -f *~