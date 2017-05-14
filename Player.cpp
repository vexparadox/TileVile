#include "Player.hpp"

Player::Player(AXVector2D position, AXVector2D size) : GameObject(position, size, true){

}

void Player::draw(){
	AXGraphics::drawRect((body->GetPosition().x*10)+size.x/2, body->GetPosition().y*10)+size.y/2, size.x, size.y);
}

void Player::tick(){
	if(AXInput::getValue("D") || AXInput::getValue("RIGHT")){
		velocity.x = 2;
	}
	if(AXInput::getValue("A") || AXInput::getValue("LEFT")){
		velocity.x = -10;
	}
}