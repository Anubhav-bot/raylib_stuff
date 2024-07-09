CARGS=-Wall -L./raylib/src/ -I./raylib/src/ -lraylib -lm -lgdi32 -lwinmm

all main.c:
	gcc main.c -o main.exe $(CARGS)

run main.exe:
	./main.exe

clean:
	rm main.exe

