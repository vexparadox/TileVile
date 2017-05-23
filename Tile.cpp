#include "Tile.hpp"

Tile::Tile(AXXMLnode_iterator& it){
	object = nullptr;
	this->id = it->attribute("id").as_int();
	this->type = it->attribute("type").as_int();
	std::string filename = it->attribute("filename").as_string();
	this->texture = new AXTexture("images/tiles/"+ filename);
	this->description = it->attribute("description").as_string();
}

Tile::Tile(const Tile* tile){
	this->id = tile->id;
	this->texture = tile->texture;
	this->type = tile->type;
	this->description = tile->description;
	this->object = tile->object;
}