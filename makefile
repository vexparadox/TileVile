all:
	g++ *.cpp -o game -std=c++11 -lSDL2 -lSDL2_mixer -lSDL2_ttf -lcurl -pthread -lAxilya
	./game
mac:
	g++ *.cpp -o game -std=c++11 -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf -lcurl  -lAxilya -rpath @executable_path/../Frameworks -mmacosx-version-min=10.10