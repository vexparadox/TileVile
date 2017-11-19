all:
	g++ *.cpp -o game -std=c++14 -g -F/Library/Frameworks -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf -lcurl  -lAxilya -rpath @executable_path/../Frameworks -mmacosx-version-min=10.10
	./game
release:
	g++ *.cpp -o game -std=c++14 -O3 -F/Library/Frameworks -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf -lcurl  -lAxilya -rpath @executable_path/../Frameworks -mmacosx-version-min=10.10
	./game