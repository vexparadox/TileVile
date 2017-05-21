#pragma once
#include <Axilya/AXMain.h>
#include "Tile.hpp"
class World;
class GUI {
	AXTexture* backgroundIMG; // background image for the GUI
	AXTexture* instructionText; // used to give instructions
	AXTexture* descriptionText; // used to describe what the user is selecting
	AXTexture* cantPlaceText; // says that it can't place
	AXTexture* moneyText; // the current cash
	AXTexture* foodText; // the current food
	AXTexture* foodLogo; // the food logo
	AXFont* fontBig;
	AXFont* fontSmall;
	World* world;
	Tile* lastTile;
	AXColour blackColour;
	int lastObjectID;
	bool onGUI; // if the mouse is over the GUI or not
	//draws the grid of objects users can pick from
	void drawObjectSelect();
	//returns if the mouse is over the GUI or not, stored in onGUI
	bool isMouseOverGUI();
	//returns the ID of which object is moused over in the GUI
	int whichObjectMousedOver();
public:
	GUI(World* world);
	void draw();
	void tick(Tile* tile);
	void updateResources();
};