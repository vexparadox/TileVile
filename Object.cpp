#include "Object.hpp"
Object::Object(int id, int food, int money, int wood, int stone, int requiredType, int cost, const std::string& filename, AXAudioChunk* placeSound, const std::string& name, const std::string& description) : id(id), food(food), money(money), wood(wood), stone(stone), requiredType(requiredType), cost(cost), name(name), description(description), placeSound(placeSound){
	texture = new AXTexture("images/objects/"+filename);
}
Object::Object(const Object* other){
	this->texture = other->texture;
	this->id = other->id;
	this->description = other->description;
	this->placeSound = other->placeSound;
	this->name = other->name;
	this->money = other->money;
	this->food = other->food;
	this->wood = other->wood;
	this->stone = other->stone;
	this->requiredType = other->requiredType;
}
