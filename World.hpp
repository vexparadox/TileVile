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
	int currentStone;
	int stoneIncome;

	//boolean if the mouse has been clicked, allows for simple clicking
	bool mouseClicked;

	AXVector2D homePosition; // where the townhall has been placed
	AXVector2D homeDistance; // the current distance to the townhall
	//which object, if any is selected
	int selectedObject;
	Tile* selectedTile; // the tile that's currently selected
	//if the home as been set
	bool homeSet;
	int allowedHomeDistance; // the allowed home distance

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

	void placeObject();
public:
	~World();
	World(int tilesize);
	void tick();
	void draw();
};