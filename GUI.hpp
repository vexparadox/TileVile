#pragma once
#include <Axilya/AXMain.h>
#include "Tile.hpp"
class World;
class GUI {
	std::vector<std::string> types; // ground type strings
	std::vector<std::string> townsizes; // ground type strings
	AXTexture* backgroundIMG; // background image for the GUI
	AXTexture* instructionText; // used to give instructions
	AXTexture* detailText1; // used to give instructions
	AXTexture* detailText2; // used to give instructions
	AXTexture* detailText3; // used to give instructions
	AXTexture* descriptionText; // used to describe what the user is selecting
	AXTexture* townNameText; // used to describe the size of the town
	AXTexture* moneyText; // the current cash
	AXTexture* foodText; // the current food
	AXTexture* foodIcon; // the food icon
	AXTexture* woodText; // the current wood
	AXTexture* woodIcon; // the wood icon
	AXTexture* stoneText; //the current stone
	AXTexture* stoneIcon; // the stone icon
	AXTexture* deleteIcon; // the delete icon
	AXTexture* popText; // the delete icon
	AXTexture* popIcon; // the delete icon
	AXFont* fontBig;
	AXFont* fontSmall;
	World* world;
	Tile* lastTileHovered;
	Tile* lastTileSelected;

	AXColour blackColour, redColour;
	int lastObjectID;
	//draws the grid of objects users can pick from
	void drawObjectSelect();
	//returns if the mouse is over the GUI or not, stored in onGUI
	bool isMouseOverGUI();
	//returns the ID of which object is moused over in the GUI
	int whichObjectMousedOver();
public:
	//Audio
	AXAudioChunk* pickupSound; // played when the user selects and object
	AXAudioChunk* cancelPickupSound; // played when the user cancels
	AXAudioChunk* destructionSound;
	AXAudioChunk* selectionSound;
	//bakes the currently selected information, boolean for if placing or not
	void bakeObjectInfoStrings(int objectID, bool placing);
	void bakeTownText();
	bool onGUI; // if the mouse is over the GUI or not
	~GUI();
	GUI(World* world);
	void draw();
	void tick(Tile* tile);
	void updateResources();
};