all:
	g++ *.cpp -o game -std=c++11 -g -framework SDL2 -framework SDL2_mixer -framework SDL2_ttf -lcurl  -lAxilya -rpath @executable_path/../Frameworks -mmacosx-version-min=10.10
	./game