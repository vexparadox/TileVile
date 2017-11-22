#include "Object.hpp"

Object::Object(AXXMLnode_iterator& it) :
cost(it->attribute("woodCost").as_int(), it->attribute("stoneCost").as_int(), it->attribute("moneyCost").as_int()),
income(it->attribute("food").as_int(), it->attribute("wood").as_int(), it->attribute("stone").as_int(), it->attribute("money").as_int(), it->attribute("pop").as_int())
	{
	this->id = it->attribute("id").as_int();
	this->requiredType = it->attribute("requires").as_int();
	this->upgradeID = it->attribute("upgradesTo").as_int();

	//get the name and description
	this->name = it->attribute("name").as_string();
	this->description = it->attribute("description").as_string();

	std::string filename = it->attribute("filename").as_string();
	this->texture = new AXTexture("images/objects/"+filename);

	std::string placeSoundFilename = "audio/placesounds/";
	placeSoundFilename.append(it->attribute("placeSound").as_string());
	this->placeSound = std::make_shared<AXAudioChunk>(placeSoundFilename);
}


Object::Object(const Object* other){
	this->texture = other->texture;
	this->id = other->id;
	this->description = other->description;
	this->placeSound = other->placeSound;
	this->name = other->name;
	this->cost = other->cost;
	this->income = other->income;
	this->requiredType = other->requiredType;
}


bool Object::HasCost(){
	return cost.wood != 0 || cost.stone != 0 || cost.money != 0;
}