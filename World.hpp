#pragma once
#include <Axilya/AXMain.h>
#include <Axilya/AXCSVLoader.hpp>
#include "Tile.hpp"
#include "GUI.hpp"

#include <random>
#include <vector>
class World{
	//the width and height of the tiles vector
	int width, height;
	//how big each tile is
	int tilesize;
	//how much energy the player has
	int playerEnergy = 12314;
	//how many tiles can fit on screen in each direction
	int maxOnScreenX, maxOnScreenY;
	bool right, left, up, down; // for inputs
	//holds all the tiles in a 2D arrayu
	std::vector<std::vector<Tile*>> map;

	//this is the tiles that are available, the map is copies of theses
	std::vector<Tile*> tiles;
	// the current offset
	AXVector2D currentOffset; 
	GUI* gui; // the GUI to be drawn
	//will fill the textures array
	void loadTextures();
	//loads the tiles
	void loadTiles();
	//loads in the map file, will do values to tileIDs 
	void loadMap();
public:
	~World();
	World(int tilesize);
	void tick();
	void draw();
};