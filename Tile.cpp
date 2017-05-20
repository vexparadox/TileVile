#include "Tile.hpp"

Tile::Tile(int id, const std::string& filename) : id(id){
	this->texture = new AXTexture("images/"+filename);
}

Tile::Tile(const Tile* tile){
	this->id = tile->id;
	this->texture = tile->texture;
}