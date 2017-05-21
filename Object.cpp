#include "Object.hpp"
Object::Object(int id, int food, int money, int requiredType, int cost, const std::string& filename, const std::string& name, const std::string& description) : id(id), food(food), money(money), requiredType(requiredType), cost(cost), name(name), description(description){
	texture = new AXTexture("images/"+filename);
}
Object::Object(const Object* other){
	this->texture = other->texture;
	this->id = other->id;
	this->description = other->description;
	this->name = other->name;
	this->money = other->money;
	this->food = other->food;
	this->requiredType = other->requiredType;
}
