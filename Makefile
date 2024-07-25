CARGS=-Wall -L./raylib/src/ -I./raylib/src/ -lraylib -lm -lgdi32 -lwinmm

all: main

main: main.c
	gcc main.c -o main.exe $(CARGS)

editor: editor.c
	gcc editor.c -o editor.exe $(CARGS)

run:
	./main.exe

clean:
	rm main.exe editor.exe

