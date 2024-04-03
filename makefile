output: main.o Image.o
	g++ main.o Image.o -o output

main.o: main.cpp
	g++ -std=c++17 -c -g main.cpp

Image.o: Image.cpp Image.h
	g++ -std=c++17 -c -g Image.cpp

clean:
	rm *.o output

all: run

run: output
	./output

.PHONY: all run