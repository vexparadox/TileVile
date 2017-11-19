#include "World.hpp"

namespace{
	int time_from_enum(TICK_SPEED speed)
	{
		switch(speed){
			case SLOW:
				return 5000;
			break;
			case MEDIUM:
				return 2500;
			break;
			case FAST:
				return 500;
			break;
		}
	}
}

World::~World(){
	//delete the textures off the original tiles and objects
	for(auto& t : tiles){
		delete t->texture;
	}
}

World::World(int tilesize) : tilesize(tilesize){

	//starting resources
	townSize = 0;
	//set the allowed home distance
	allowedHomeDistance = 5;

	//set the offset
	currentOffset.x = 14;
	currentOffset.y = 14;

	//set how many can be on screen at once
	maxOnScreenX = AXWindow::getWidth()/tilesize;
	maxOnScreenY = (AXWindow::getHeight()/tilesize)-guiTileSize;
	selectedTile = nullptr;

	mouseClicked = false;

	//load tiles in
	loadTiles();
	//load in objects
	loadObjects();

	//if the home as been picked
	homeSet = false;
	selectedObject = objects.at(0); // pick the town center

	//load in the map and genertae
	loadMap();

	if(height < maxOnScreenY || width < maxOnScreenX){
		AXLog::log("World", "You can't make a map that's smaller than the screen.", AX_LOG_ERROR);
	}
	//create the GUI
	gui = std::make_unique<GUI>(this); // has to be last, it uses object and tile arrays
	timer.start();
}

void World::draw(){
	int row = 0;
	//this loop will go from the offset to the maxonScreen+offset, meaning it draws the visible porition of the world
	for(int i = currentOffset.x; i < maxOnScreenX+currentOffset.x; i++){
		int col = 0;
		for(int j = currentOffset.y; j < maxOnScreenY+currentOffset.y; j++){
			//draw the background
			Tile* curr_tile = map[i][j];
			AXGraphics::drawTexture(curr_tile->texture, (row*tilesize), (col*tilesize), tilesize, tilesize);
			if(curr_tile->object){
				AXGraphics::drawTexture(curr_tile->object->texture, (row*tilesize), (col*tilesize), tilesize, tilesize);
			}
			//if currently selected, highlight it
			if(selectedTile){
				if(selectedTile == curr_tile){
					AXGraphics::fill(0, 255, 0, 50);
					AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
				}
			}

			//if the home has been set, cover the area you can't place
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
				if(selectedObject){
					//if it's placeable, make it red!
					if(selectedObject->requiredType != curr_tile->type || curr_tile->object){
						AXGraphics::fill(255, 0, 0, 200);
					}else{ // else make it green
						AXGraphics::fill(0, 255, 0, 150);
					}
				}else{
					//or if we don't have an object selected
					//just highlight slightly
					AXGraphics::fill(0, 255, 0, 100);
				}
				AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
			}
			col++; // advance the col draw position
		}
		row++; // advance the row draw position
	}
	
	AXGraphics::fill(255, 255, 255, 255);
	if(selectedObject){
		AXGraphics::drawTexture(selectedObject->texture, (mousePosition.x*tilesize), (mousePosition.y*tilesize), tilesize, tilesize);
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

	//get the current distance from the mouse to home, this is used for placing
	if(homeSet){
		homeDistance.x = homePosition.x - (mousePosition.x+currentOffset.x);
		homeDistance.y = homePosition.y - (mousePosition.y+currentOffset.y);
	}

	HandleMouseClicks();
	
	//gui takes a tile pointer, this is what the user is moused over
	gui->tick(getMousedTile());
	if(timer.elapsedTime() > time_from_enum(current_tick_speed)){
		timer.reset();
		timer.start();
		inGameTick();
	}
}

void World::HandleMouseClicks()
{
	// if the mouse is pressed on the map
	if(AXInput::getValue("MB1") && AXInput::mouseY < maxOnScreenY*tilesize && !mouseClicked){
		mouseClicked = true;
		if(selectedObject){
			//check if the type is correct
			//if there's already an object
			//if we can afford
			//and we're close enough to the home
			if(getMousedTile()->type == selectedObject->requiredType 
				&& !getMousedTile()->object 
				&& resource_pool.CanAfford(selectedObject->cost))
				{
				//if the home has been set check the distance
				if(homeSet){
					if(InHomeBounds()){
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
			//you can then select the tile IF it has an object on it
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

}

bool World::InHomeBounds()
{
	return AXMath::absolute(homeDistance.x) <= allowedHomeDistance && AXMath::absolute(homeDistance.y) <= allowedHomeDistance;
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
	AXXML xml("XML/Tiles.xml");
	AXXMLnode tilenode = xml.child("tiles");
	//loop through the "tiles"
	for (AXXMLnode_iterator it = tilenode.begin(); it != tilenode.end(); ++it){
	    tiles.push_back(new Tile(it));
	}
}
void World::loadObjects(){
	AXXML xml("XML/Objects.xml");
	AXXMLnode objectnode = xml.child("objects");
	//loop through the "tiles"
	for (AXXMLnode_iterator it = objectnode.begin(); it != objectnode.end(); ++it){
		objects.push_back(std::make_shared<Object>(it));
	}
}

void World::placeObject(){
	//update the tile with the selected object
	getMousedTile()->object = selectedObject;
	auto curr_obj = selectedObject;
	//get the incomes from the objects
	resource_pool.GainIncome(curr_obj->income);
	//take away the cost
	resource_pool.Spend(curr_obj->cost);

	AXAudio::playAudioChunk(curr_obj->placeSound.get());
	this->townSize = getTownSize();
	//update the incomes
	gui->updateResources();
	gui->bakeTownText();
	//turn it so we're not holding it anymore
	selectedObject.reset();
}

void World::deleteObject(){
	auto curr_obj = selectedTile->object;
	//lose the income of the object
	resource_pool.LoseIncome(curr_obj->income);	
	//get the income of half the original cost
	resource_pool.GainMoney(curr_obj->cost.money/2);
	AXAudio::playAudioChunk(gui->destructionSound);
	this->townSize = getTownSize();
	//update the incomes
	gui->updateResources();
	gui->bakeTownText();
	//turn it so we're not holding it anymore
	selectedTile->object.reset();
	selectedTile = nullptr;
}

int World::getTownSize(){
	int currentPop = resource_pool.population;
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
	resource_pool.ProcessIncome();
	gui->updateResources();
}
