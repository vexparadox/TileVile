#pragma once
#include <Axilya/AXMain.h>
#include "Object.hpp"
class Tile{
public:
	int id, type;
	AXTexture* texture; // the texture for this tile
	std::shared_ptr<Object> object; // the object that's on this tile
	std::string description; // the description of this tile
	Tile(AXXMLnode_iterator& it); 
	Tile(const Tile* tile); // used to copy tiles
};