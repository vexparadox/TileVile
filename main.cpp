#include "main.hpp"
World* world;
int main(int argc, char const *argv[]){
	AXWindow::init(1280, 720, "Game", AX_DEFAULT | AX_NOAUDIO, setup, update, draw);
	int ret = AXWindow::run();
	delete world;
	return ret;
}

void setup(){
	// new world(width, height, tilesize);
	world = new World(200, 200, 40);
}

void draw(){
	//draws the world
	world->draw();
}

void update(){
	//will allow the player to move the world around
	world->tick();
}