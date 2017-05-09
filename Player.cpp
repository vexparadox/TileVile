#include "Player.hpp"

Player::Player(AXVector2D position, AXVector2D size) : GameObject(position, size, true){

}

void Player::draw(){
	AXGraphics::drawRect(position.x, position.y, size.x, size.y);
}

void Player::tick(){
	if(AXInput::getValue("D") || AXInput::getValue("RIGHT")){
		velocity.x = 2;
	}
	if(AXInput::getValue("A") || AXInput::getValue("LEFT")){
		velocity.x = -10;
	}
}