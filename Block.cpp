#include "Block.hpp"

Block::Block(AXVector2D position, AXVector2D size) : GameObject(position, size, true){

}

void Block::draw(){
	AXGraphics::drawRect(position.x, position.y, size.x, size.y);
}

void Block::tick(){
	// if(AXInput::getValue("D") || AXInput::getValue("RIGHT")){
		// position.x += 2;
	// }
}