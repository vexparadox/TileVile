#pragma once
#include <Axilya/AXMain.h>
class Tile{
public:
	int id;
	AXTexture* texture;
	Tile(int id, const std::string& filename);
	Tile(const Tile* tile);
};