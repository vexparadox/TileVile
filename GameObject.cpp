#include "GameObject.hpp"

GameObject::GameObject(AXVector2D position, AXVector2D size, bool gravity) : position(position), size(size), gravity(gravity){

}

void GameObject::physics(){
	//terminal velocity
	float maxVelocityX = 7;
	float maxVelocityY = 7;
	float maxFallingVelocity = 10;

    if(velocity.x > maxVelocityX){
        velocity.x = maxVelocityX;
    }else if(velocity.x < - maxVelocityX){
        velocity.x = -maxVelocityX;
    }
    if(velocity.y > maxFallingVelocity){
        velocity.y = maxFallingVelocity;
    }
    if(velocity.y < -maxVelocityY){
        velocity.y = -maxVelocityY;
    }
    //apply velocity
	this->position += velocity;
	//apply gravity
	if(gravity){
		velocity.y += 5;
	}
	AXVector2D drag(0.5, 0.5);
	//apply drag
	if(velocity.x < drag.x && velocity.x > drag.x){
	    velocity.x = 0;
	}else if(velocity.y > 0){
	    velocity.y -= drag.y;
	}else if(velocity.y < 0){
	    velocity.y += drag.y;
	}
	if(velocity.y < drag.y && velocity.y > drag.y){
	    velocity.y = 0;
	}else if(velocity.x > 0){
	    velocity.x -= drag.x;
	}else if(velocity.x < 0){
	    velocity.x += drag.x;
	}
}

AXVector2D& GameObject::getPos(){
	return this->position;
}
AXVector2D& GameObject::getSize(){
	return this->size;
}

void GameObject::setPos(float x, float y){
	this->position.x = x;
	this->position.y = y;
}

void GameObject::setSize(float x, float y){
	this->size.x = x;
	this->size.y = y;
}
