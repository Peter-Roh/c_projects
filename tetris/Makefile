CC=clang
CFLAGS=-W -pedantic-errors

tetris: tetris.o
	clang tetris.o `pkg-config --libs raylib` -o tetris

tetris.o:
	clang -c `pkg-config --cflags raylib` tetris.c

clean:
	rm tetris.o tetris
