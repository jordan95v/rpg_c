EXE=prog
CC=gcc

$(EXE): main.o map.o player.o functions.o
	$(CC) -o $@ $^ -lSDL2 -lSDL2_image -lSDL2_ttf
	rm -rf *.o

main.o: main.c
map.o: map.c map.h
player.o: player.c player.h
functions.o: functions.c functions.h

%.o: %.c %.h
	$(CC) -c $<

install:
	apt install gcc libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev -y

clean:
	rm -rf *.o $(EXE)