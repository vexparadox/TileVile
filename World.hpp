#pragma once
#include <Axilya/AXMain.h>
#include "Tile.hpp"
#include <random>
#include <vector>
class World{
	int width, height, tilesize;
	std::vector<std::vector<Tile*>> tiles;
public:
	AXVector2D currentCenter;
	World(int width, int height, int tilesize);
	void tick();
	void draw();
};