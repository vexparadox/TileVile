#include "Tile.hpp"

Tile::Tile(int id, const std::string& filename, const std::string& description, bool placeable) : id(id), description(description), placeable(placeable){
	object = nullptr;
	this->texture = new AXTexture("images/"+filename);
}

Tile::Tile(const Tile* tile){
	this->id = tile->id;
	this->texture = tile->texture;
	this->placeable = tile->placeable;
	this->description = tile->description;
	this->object = tile->object;
}