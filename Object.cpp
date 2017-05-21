#include "Object.hpp"
Object::Object(int id, int food, int money, int cost, const std::string& filename, const std::string& description) : id(id), food(food), money(money), cost(cost), description(description){
	texture = new AXTexture("images/"+filename);
}
Object::Object(const Object* other){
	this->texture = other->texture;
	this->id = other->id;
	this->description = other->description;
	this->money = other->money;
	this->food = other->food;
}
