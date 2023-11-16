build:
	gcc -g -Wall ./src/*.c -o emulator -lSDL2 -lSDL2_mixer

run:
	./emulator

clean:
	rm emulator
