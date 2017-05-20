#include "GUI.hpp"
#include "World.hpp"
GUI::GUI(World* world) : world(world){
	font = new AXFont("font/Ayuthaya.ttf", 20);
	positionText = nullptr;
}

void GUI::tick(){

}

void GUI::draw(){
	if(positionText){
		AXGraphics::drawTexture(positionText, 30, AXWindow::getHeight()-positionText->getHeight()-20); 
	}
}

void GUI::updatePosition(const AXVector2D& offset){
	if(positionText){
		delete positionText;
	}
	AXColour colour(0, 0, 0);
	positionText = font->quickBakeTexture("Position:("+std::to_string((int)offset.x)+", "+std::to_string((int)offset.y)+")", colour);
}