#include "main.hpp"
World* world;
int main(int argc, char const *argv[]){
	AXWindow::init(1280, 720, "Game", AX_DEFAULT, setup, update, draw);
	return AXWindow::run();
}

void setup(){
	world = new World(1920/40, 1280/40, 40);
}

void draw(){
	world->draw();
}

void update(){
	if(AXInput::getValue("A")){
		if(world->currentCenter.x != 0){
			world->currentCenter.x--;
		}
	}

	if(AXInput::getValue("D")){
		if((AXWindow::getWidth()/40)+world->currentCenter.x < 1920/40){
			world->currentCenter.x++;
		}
	}
	if(AXInput::getValue("W")){
		if(world->currentCenter.y != 0){
			world->currentCenter.y--;
		}
	}

	if(AXInput::getValue("S")){
		if((AXWindow::getHeight()/40)+world->currentCenter.y < 1280/40){
			world->currentCenter.y++;
		}
	}
}