#pragma once
#include <Axilya/AXMain.h>
#include "Tile.hpp"
#include <random>
#include <vector>
class World{
	//the width and height of the tiles vector
	int width, height;
	//how big each tile is
	int tilesize;
	//how many tiles can fit on screen in each direction
	int maxOnScreenX, maxOnScreenY;
	bool right, left, up, down; // for inputs
	//holds all the tiles in a 2D arrayu
	std::vector<std::vector<Tile*>> tiles;
	//textures
	std::vector<AXTexture*> textures;
	//the player texture
	AXTexture* playerTexture;
	
	AXVector2D playerPosition;
	AXVector2D currentOffset; // the current offset

	//will fill the textures array
	void loadTextures();
public:
	~World();
	World(int width, int height, int tilesize);
	void tick();
	void draw();
};