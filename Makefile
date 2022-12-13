MAKE := make
all:
	$(MAKE) -C ./tetris

clean:
	rm ./*/*.o ./tetris/tetris
