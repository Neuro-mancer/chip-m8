build:
	gcc -g -Wall ./src/*.c -o emulator -lSDL2

run:
	./emulator

clean:
	rm emulator
