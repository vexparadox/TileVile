boxsrc = $(shell find Box2D/**/*.cpp)

all:
	g++ *.cpp -o game -std=c++11 -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcurl -pthread -lAxilya 
	./game