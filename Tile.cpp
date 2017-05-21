#include "Tile.hpp"

Tile::Tile(int id, const std::string& filename, const std::string& description, int type) : id(id), description(description), type(type){
	object = nullptr;
	this->texture = new AXTexture("images/"+filename);
}

Tile::Tile(const Tile* tile){
	this->id = tile->id;
	this->texture = tile->texture;
	this->type = tile->type;
	this->description = tile->description;
	this->object = tile->object;
}