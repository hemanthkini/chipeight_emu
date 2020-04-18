all: main.c
	clang main.c -o chip8 -framework SDL2

clean:
	rm -f ./chip8; rm -f *~