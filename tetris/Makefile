CC=clang
CFLAGS=-W -pedantic-errors

all: clean tetris

tetris: tetris.o
	clang tetris.o `pkg-config --libs raylib` -o tetris

tetris.o:
	clang -c `pkg-config --cflags raylib` tetris.c

clean:
	rm tetris.o tetris
