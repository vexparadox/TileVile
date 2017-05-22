#pragma once
#include <Axilya/AXMain.h>
#include <Axilya/AXCSVLoader.hpp>
#include "Tile.hpp"
#include "Object.hpp"
#include "GUI.hpp"
#include <random>
#include <vector>
class World{
	friend class GUI;
	//the width and height of the map vector
	int width, height;
	//how big each tile is
	int tilesize;
	//how many tiles can fit on screen in each direction
	int maxOnScreenX, maxOnScreenY;	

	//player resources
	int currentMoney;
	int moneyIncome;
	int currentFood;
	int foodIncome;
	int currentWood;
	int woodIncome;

	//holds all the tiles in a 2D arrayu
	std::vector<std::vector<Tile*>> map;
	//this is the tiles that are available, the map is copies of theses
	std::vector<Tile*> tiles;
	//if the objects that have been loaded
	std::vector<Object*> objects;
	// the current offset
	AXVector2D currentOffset; 
	//which tile the mouse is on
	AXVector2D mousePosition;
	// the GUI to be drawn
	GUI* gui;
	//timer used to call inGameTick
	AXTimer timer;
	//loads the tiles
	void loadTiles();
	//load the objects
	void loadObjects();
	//loads in the map file, will do values to tileIDs 
	void loadMap();
	//returns the tile pointer to the tile the mouse is over
	Tile* getMousedTile();
	void inGameTick();
public:
	//which object, if any is selected
	int selectedObject;
	//if the home as been set
	bool homeSet;
	~World();
	World(int tilesize);
	void tick();
	void draw();
};