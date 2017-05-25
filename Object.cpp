#include "Object.hpp"

Object::Object(AXXMLnode_iterator& it){
	this->id = it->attribute("id").as_int();
	this->food = it->attribute("food").as_int();
	this->moneyCost = it->attribute("moneyCost").as_int();
	this->stoneCost = it->attribute("stoneCost").as_int();
	this->woodCost = it->attribute("woodCost").as_int();
	this->wood = it->attribute("wood").as_int();
	this->pop = it->attribute("pop").as_int();
	this->stone = it->attribute("stone").as_int();
	this->requiredType = it->attribute("requires").as_int();
	this->money = it->attribute("money").as_int();

	//get the name and description
	this->name = it->attribute("name").as_string();
	this->description = it->attribute("description").as_string();

	std::string filename = it->attribute("filename").as_string();
	this->texture = new AXTexture("images/objects/"+filename);

	std::string placeSoundFilename = "audio/placesounds/";
	placeSoundFilename.append(it->attribute("placeSound").as_string());
	this->placeSound = new AXAudioChunk(placeSoundFilename);
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
