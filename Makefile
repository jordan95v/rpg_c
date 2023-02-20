EXE=prog
CC=gcc

$(EXE): main.o map.o player.o functions.o
	$(CC) -o $@ $^ -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
	rm -rf *.o

main.o: main.c
map.o: map.c map.h
player.o: player.c player.h
functions.o: functions.c functions.h

%.o: %.c %.h
	$(CC) -c $<

install:
	apt install gcc libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev -y

uninstall:
	apt remove libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev  libsdl2-mixer-dev -y

clean:
	rm -rf *.o $(EXE)