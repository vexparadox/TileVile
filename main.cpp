#include "main.hpp"
World* world;
Menu* menu;
int gameState;
AXAudioChunk* mainTheme;
int main(int argc, char const *argv[]){
	AXWindow::init(1200, 840, "Game", AX_DEFAULT, setup, update, draw);
	int ret = AXWindow::run();
	delete world;
	delete mainTheme;
	return ret;
}

void setup(){
	gameState = 1;
	// new world(width, height, tilesize);
	world = new World(60);
	menu = new Menu();
	mainTheme = new AXAudioChunk("audio/theme.wav");
	AXAudio::playAudioChunk(mainTheme, 0, -1);
}

void draw(){
	switch(gameState){
		case 0:
			menu->draw();
		break;
		case 1:
			world->draw();
		break;
	}
}

void update(){
	switch(gameState){
		case 0:
			menu->tick();
		break;
		case 1:
			world->tick();
		break;
	}
}