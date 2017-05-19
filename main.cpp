#include "main.hpp"
World* world;
Menu* menu;
int gameState;

int main(int argc, char const *argv[]){
	AXWindow::init(1200, 720, "Game", AX_DEFAULT | AX_NOAUDIO, setup, update, draw);
	int ret = AXWindow::run();
	delete world;
	return ret;
}

void setup(){
	gameState = 0;
	// new world(width, height, tilesize);
	world = new World(200, 200, 60);
	menu = new Menu();
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