#pragma once
#include <Axilya/AXMain.h>
#include "Object.hpp"
class Tile{
public:
	int id, type;
	AXTexture* texture; // the texture for this tile
	Object* object; // the object that's on this tile
	std::string description; // the description of this tile
	Tile(int id, const std::string& filename, const std::string& description, int type);
	Tile(const Tile* tile); // used to copy tiles
};