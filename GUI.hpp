#pragma once
#include <Axilya/AXMain.h>
#include "Text.hpp"
#include "Tile.hpp"
#include <memory>
class World;
class GUI {
	std::vector<std::string> types; // ground type strings
	std::vector<std::string> townsizes; // ground type strings
	std::unique_ptr<AXTexture> backgroundIMG; // background image for the GUI

	std::unique_ptr<Text> instructionText; // used to give instructions
	std::unique_ptr<Text> detailText1; // used to give instructions
	std::unique_ptr<Text> detailText2; // used to give instructions
	std::unique_ptr<Text> detailText3; // used to give instructions
	std::unique_ptr<Text> descriptionText; // used to describe what the user is selecting
	std::unique_ptr<Text> worldSpeedText; // used to describe the current tick speed
	std::unique_ptr<Text> townNameText; // used to describe the size of the town
	std::unique_ptr<Text> moneyText; // the current cash
	std::unique_ptr<Text> foodText; // the current food
	std::unique_ptr<Text> woodText; // the current wood
	std::unique_ptr<Text> stoneText; //the current stone
	std::unique_ptr<Text> popText; // the population text

	std::unique_ptr<AXTexture> foodIcon; // the food icon
	std::unique_ptr<AXTexture> woodIcon; // the wood icon
	std::unique_ptr<AXTexture> stoneIcon; // the stone icon
	std::unique_ptr<AXTexture> deleteIcon; // the delete icon	
	std::unique_ptr<AXTexture> popIcon; // the delete icon

	std::shared_ptr<AXFont> fontBig;
	std::shared_ptr<AXFont> fontSmall;
	World* world;
	Tile* lastTileHovered;
	Tile* lastTileSelected;
	
	AXColour blackColour, redColour;
	std::shared_ptr<Object> lastObjectID;
	//draws the grid of objects users can pick from
	void drawObjectSelect();
	//returns if the mouse is over the GUI or not, stored in onGUI
	bool isMouseOverGUI();
	//returns the object which moused over in the GUI
	std::shared_ptr<Object> whichObjectMousedOver();
public:
	//Audio
	std::unique_ptr<AXAudioChunk> pickupSound; // played when the user selects and object
	std::unique_ptr<AXAudioChunk> cancelPickupSound; // played when the user cancels
	std::unique_ptr<AXAudioChunk> destructionSound;
	std::unique_ptr<AXAudioChunk> selectionSound;
	//bakes the currently selected information, boolean for if placing or not
	void bakeObjectInfoStrings(std::shared_ptr<Object>, bool placing);
	void bakeTownText();
	void bakeSpeedText();
	bool onGUI; // if the mouse is over the GUI or not
	GUI(World* world);
	void draw();
	void tick(Tile* tile);
	void updateResources();
};