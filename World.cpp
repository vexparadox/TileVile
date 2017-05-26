#include "World.hpp"

World::~World(){
	//delete the textures off the original tiles and objects
	for(auto& t : tiles){
		delete t->texture;
	}
	for(auto& o : objects){
		delete o->texture;
	}
	//remove the GUI
	delete gui;
}

World::World(int tilesize) : tilesize(tilesize){
	//if the home as been picked
	homeSet = false;
	selectedObject = 0; // the town center
	//starting resources
	currentMoney = 600;
	moneyIncome = 0;
	currentFood = 20;
	foodIncome = 0;
	currentWood = 200;
	woodIncome = 0;
	currentStone = 150;
	stoneIncome = 0;
	currentPop = 0;
	townSize = 0;
	//set the allowed home distance
	allowedHomeDistance = 5;

	//set the offset
	currentOffset.x = 14;
	currentOffset.y = 14;

	//set how many can be on screen at once
	maxOnScreenX = AXWindow::getWidth()/tilesize;
	maxOnScreenY = (AXWindow::getHeight()/tilesize)-2;
	selectedTile = nullptr;

	mouseClicked = false;

	//load tiles in
	loadTiles();
	//load in objects
	loadObjects();

	//load in the map and genertae
	loadMap();

	if(height < maxOnScreenY || width < maxOnScreenX){
		AXLog::log("World", "You can't make a map that's smaller than the screen.", AX_LOG_ERROR);
	}
	//create the GUI
	gui = new GUI(this); // has to be last, it uses object and tile arrays
	timer.start();
}

void World::draw(){
	int row = 0;
	//this loop will go from the offset to the maxonScreen+offset, meaning it draws the visible porition of the world
	for(int i = currentOffset.x; i < maxOnScreenX+currentOffset.x; i++){
		int col = 0;
		for(int j = currentOffset.y; j < maxOnScreenY+currentOffset.y; j++){
			//draw the background
			AXGraphics::drawTexture(map[i][j]->texture, (row*tilesize), (col*tilesize), tilesize, tilesize);
			if(map[i][j]->object){
				AXGraphics::drawTexture(map[i][j]->object->texture, (row*tilesize), (col*tilesize), tilesize, tilesize);
			}
			//if currently selected, highlight it
			if(selectedTile){
				if(selectedTile == map[i][j]){
					AXGraphics::fill(0, 255, 0, 50);
					AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
				}
			}

			//if the home has been set, and you're placing
			//cover the area you can't place
			if(homeSet){
				if(AXMath::absolute(i-homePosition.x) > allowedHomeDistance || AXMath::absolute(j-homePosition.y) > allowedHomeDistance){
					AXGraphics::fill(100, 100, 100, 150);
					AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
				}			
			}else{
				//else if the home hasn't been set, show the workable area for the townhall
				if(AXMath::absolute(i-(mousePosition.x+currentOffset.x)) <= allowedHomeDistance && AXMath::absolute(j-(mousePosition.y+currentOffset.y)) <= allowedHomeDistance){
					AXGraphics::fill(0, 255, 0, 100);
					AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
				}
			}

			//mouse over!
			if(i == mousePosition.x+currentOffset.x && j == mousePosition.y+currentOffset.y && !gui->onGUI){
				if(selectedObject >= 0){
					//if it's placeable, make it red!
					if(objects[selectedObject]->requiredType != map[i][j]->type || map[i][j]->object){
						AXGraphics::fill(255, 0, 0, 200);
					}else{
						AXGraphics::fill(0, 255, 0, 150);
					}
				}else{
					AXGraphics::fill(0, 255, 0, 100);
				}
				AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
			}
			col++; // advance the col draw position
		}
		row++; // advance the row draw position
	}
	
	AXGraphics::fill(255, 255, 255, 255);
	if(selectedObject >= 0){
		AXGraphics::drawTexture(objects[selectedObject]->texture, (mousePosition.x*tilesize), (mousePosition.y*tilesize), tilesize, tilesize);
	}

	//draw the gui
	gui->draw();
}

void World::tick(){
	// move the player, it will be corrected later
	// this uses step movement
	if(AXInput::getValue("A") || AXInput::getValue("LEFT")){
		if(currentOffset.x != 0){
			currentOffset.x--;
		}
	}

	if(AXInput::getValue("D") || AXInput::getValue("RIGHT")){
		if(maxOnScreenX+currentOffset.x < width){
			currentOffset.x++;
		}
	}

	if(AXInput::getValue("W") || AXInput::getValue("UP")){
		if(currentOffset.y != 0){
			currentOffset.y--;
		}
	}

	if(AXInput::getValue("S") || AXInput::getValue("DOWN")){
		if(maxOnScreenY+currentOffset.y < height){
			currentOffset.y++;
		}
	}
	//get the mouse position (tile which the mouse is over)
	mousePosition.x = AXInput::mouseX/tilesize;
	mousePosition.y = AXInput::mouseY/tilesize;
	//make sure it's still on screen
	if(mousePosition.x >= maxOnScreenX){
		mousePosition.x = maxOnScreenX-1;
	}
	if(mousePosition.y >= maxOnScreenY){
		mousePosition.y = maxOnScreenY-1;
	}

	//get the current distance from the mouse to home
	if(homeSet){
		homeDistance.x = homePosition.x - (mousePosition.x+currentOffset.x);
		homeDistance.y = homePosition.y - (mousePosition.y+currentOffset.y);
	}


	// if the mouse is pressed on the map
	if(AXInput::getValue("MB1") && AXInput::mouseY < maxOnScreenY*tilesize && !mouseClicked){
		mouseClicked = true;
		if(selectedObject >= 0){
			//check if the type is correct
			//if there's already an object
			//if we can afford
			//and we're close enough to the home
			if(getMousedTile()->type == objects[selectedObject]->requiredType 
				&& !getMousedTile()->object 
				&& objects[selectedObject]->moneyCost <= currentMoney
				&& objects[selectedObject]->woodCost <= currentWood
				&& objects[selectedObject]->stoneCost <= currentStone){
				//if the home has been set check the distance
				if(homeSet){
					if(AXMath::absolute(homeDistance.x) <= allowedHomeDistance && AXMath::absolute(homeDistance.y) <= allowedHomeDistance){
						placeObject();
					}
				}else{
					placeObject();
					homePosition = AXVector2D(mousePosition.x+currentOffset.x, mousePosition.y+currentOffset.y);
					homeSet = true;
				}
			}
		}else{
			//if you have nothing selected
			if(getMousedTile()->object){
				selectedTile = getMousedTile();
				AXAudio::playAudioChunk(gui->selectionSound);
			}else if(selectedTile != nullptr){
				selectedTile = nullptr;
				AXAudio::playAudioChunk(gui->cancelPickupSound);
			}
		}
	}
	if(!AXInput::getValue("MB1") && mouseClicked){
		mouseClicked = false;
	}

	//gui takes a tile pointer, this is what the user is moused over
	gui->tick(getMousedTile());
	if(timer.elapsedTime() > 2000){
		timer.reset();
		timer.start();
		inGameTick();
	}
}

void World::loadMap(){
	AXCSVLoader loader("MAP/map1.csv");
	std::vector<std::vector<int> >& data = loader.loadFile();
	this->width = data.size(); // must be the same width constantly
	this->height = data[0].size(); // must be the same height constantly

	for(int i = 0; i < width; i++){
		std::vector<Tile*> vec;
		map.push_back(vec);
		for(int j = 0; j < height; j++){
			map[i].push_back(new Tile(tiles[data[i][j]]));
		}
	}
}

void World::loadTiles(){
	AXXML xml("XML/tiles.xml");
	AXXMLnode tilenode = xml.child("tiles");
	//loop through the "tiles"
	for (AXXMLnode_iterator it = tilenode.begin(); it != tilenode.end(); ++it){
	    tiles.push_back(new Tile(it));
	}
}
void World::loadObjects(){
	AXXML xml("XML/objects.xml");
	AXXMLnode objectnode = xml.child("objects");
	//loop through the "tiles"
	for (AXXMLnode_iterator it = objectnode.begin(); it != objectnode.end(); ++it){
		objects.push_back(new Object(it));
	}
}

void World::placeObject(){
	//update the tile with the selected object
	getMousedTile()->object = new Object(objects[selectedObject]);
	//get the incomes from the objects
	moneyIncome += objects[selectedObject]->money;
	foodIncome += objects[selectedObject]->food;
	woodIncome += objects[selectedObject]->wood;
	stoneIncome += objects[selectedObject]->stone;
	currentPop += objects[selectedObject]->pop;
	//take away the cost
	currentMoney -= objects[selectedObject]->moneyCost;
	currentWood -= objects[selectedObject]->woodCost;
	currentStone -= objects[selectedObject]->stoneCost;
	AXAudio::playAudioChunk(objects[selectedObject]->placeSound);
	this->townSize = getTownSize();
	//update the incomes
	gui->updateResources();
	gui->bakeTownText();
	//turn it so we're not holding it anymore
	selectedObject = -1;
}

void World::deleteObject(){
	//get the incomes from the objects
	int selectedID = selectedTile->object->id;
	moneyIncome -= objects[selectedID]->money;
	foodIncome -= objects[selectedID]->food;
	woodIncome -= objects[selectedID]->wood;
	stoneIncome -= objects[selectedID]->stone;
	currentPop -= objects[selectedID]->pop;
	//take away the cost
	currentMoney += (int)objects[selectedID]->moneyCost/2;
	AXAudio::playAudioChunk(gui->destructionSound);
	this->townSize = getTownSize();
	//update the incomes
	gui->updateResources();
	gui->bakeTownText();
	//turn it so we're not holding it anymore
	delete selectedTile->object;
	selectedTile->object = nullptr;
	selectedTile = nullptr;
}

int World::getTownSize(){
	if(currentPop < 0){
		gameState = 0;
		return 0;
	}else if(currentPop > 0 && currentPop <= 60){
		allowedHomeDistance = 5;
		return 0;
	}else if(currentPop > 60 && currentPop <= 250){
		allowedHomeDistance = 8;
		return 1;
	}else if(currentPop > 250 && currentPop <= 600){
		allowedHomeDistance = 20;
		return 2;
	}else if(currentPop > 600){
		allowedHomeDistance = 100;
		return 3;
	}
	return 3;
}

Tile* World::getMousedTile(){
	return map[mousePosition.x+currentOffset.x][mousePosition.y+currentOffset.y];
}

void World::inGameTick(){
	currentMoney += moneyIncome;
	currentFood += foodIncome;
	currentWood += woodIncome;
	currentStone += stoneIncome;
	//people die with no food
	if(currentFood < 0){
		currentPop -= 2;
	}
	gui->updateResources();
}