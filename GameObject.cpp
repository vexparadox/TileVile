#include "GameObject.hpp"

GameObject::GameObject(AXVector2D position, AXVector2D size, bool gravity) : position(position), size(size), gravity(gravity), body(0){
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(position.x/10, position.y/10);
	body = world->CreateBody(&bodyDef);

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox((size.x/10)-1, (size.y/10)-1);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 1.0f;
	body->CreateFixture(&fixtureDef);

}


AXVector2D GameObject::getPos(){
	return AXVector2D(this->body->GetPosition().x, this->body->GetPosition().y);
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
