EXE=main
CC=gcc

$(EXE): main.o map.o
	$(CC) -o $@ $^ -lSDL2 -lSDL2_image
	rm -rf *.o

main.o: main.c
map.o: map.c map.h

%.o: %.c %.h
	$(CC) -c $<

clean:
	rm -rf *.o $(EXE)