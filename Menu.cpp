#include "Menu.hpp"
Menu::Menu(){

}

void Menu::draw(){
	AXGraphics::setBackground(0, 155, 0);

}

void Menu::tick(){
	if(AXInput::getValue("ENTER")){
		gameState = 1;
	}
}