boxsrc = $(shell find Box2D/**/*.cpp)

all:
	g++ *.cpp -std=c++11 -L. -I./Box2D -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcurl -pthread -lAxilya -lBox2D 
	./a.out