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
	//starting cash
	currentMoney = 400;
	moneyIncome = 0;
	currentFood = 20;
	foodIncome = 0;
	currentWood = 20;
	woodIncome = 0;


	//set the offset
	currentOffset.x = 14;
	currentOffset.y = 14;
	//create the GUI
	gui = new GUI(this);

	//set how many can be on screen at once
	maxOnScreenX = AXWindow::getWidth()/tilesize;
	maxOnScreenY = (AXWindow::getHeight()/tilesize)-2;

	//load tiles in
	loadTiles();
	//load in objects
	loadObjects();

	//load in the map and genertae
	loadMap();

	if(height < maxOnScreenY || width < maxOnScreenX){
		AXLog::log("World", "You can't make a map that's smaller than the screen.", AX_LOG_ERROR);
	}
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
	if(AXInput::getValue("A")){
		if(currentOffset.x != 0){
			currentOffset.x--;
		}
	}

	if(AXInput::getValue("D")){
		if(maxOnScreenX+currentOffset.x < width){
			currentOffset.x++;
		}
	}

	if(AXInput::getValue("W")){
		if(currentOffset.y != 0){
			currentOffset.y--;
		}
	}

	if(AXInput::getValue("S")){
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


	//place the object selected IF
	// the mouse is pressed
	// and the mouse is on the tiles
	if(selectedObject >= 0 && AXInput::getValue("MB1") && AXInput::mouseY < maxOnScreenY*tilesize){
		//check if we can place and afford
		if(getMousedTile()->type == objects[selectedObject]->requiredType && !getMousedTile()->object && objects[selectedObject]->cost <= currentMoney){	
			//update the tile with the selected object
			getMousedTile()->object = new Object(objects[selectedObject]);
			//get the incomes from the objects
			moneyIncome += objects[selectedObject]->money;
			foodIncome += objects[selectedObject]->food;
			woodIncome += objects[selectedObject]->wood;
			//take away the cost
			currentMoney -= objects[selectedObject]->cost;
			AXAudio::playAudioChunk(objects[selectedObject]->placeSound);
			//update the incomes
			gui->updateResources();
			//make sure we ain't holding one
			selectedObject = -1;
			//if placing and there's no home set, we can assume this is the home being set
			if(!homeSet){
				homeSet = true;
			}
		}
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
		//for each tile, get the ID and filename attributes
		int id = it->attribute("id").as_int();
		int type = it->attribute("type").as_int();
		std::string filename = it->attribute("filename").as_string();
		std::string description = it->attribute("description").as_string();
	    tiles.push_back(new Tile(id, filename, description, type));
	}
}
void World::loadObjects(){
	AXXML xml("XML/objects.xml");
	AXXMLnode objectnode = xml.child("objects");
	//loop through the "tiles"
	for (AXXMLnode_iterator it = objectnode.begin(); it != objectnode.end(); ++it){
		//for each tile, get the id, costs, wood and food production, or not
		int id = it->attribute("id").as_int();
		int food = it->attribute("food").as_int();
		int cost = it->attribute("cost").as_int();
		int wood = it->attribute("wood").as_int();
		int requiredType = it->attribute("requires").as_int();
		int money = it->attribute("money").as_int();

		std::string filename = it->attribute("filename").as_string();

		std::string placeSoundFilename = "audio/placesounds/";
		placeSoundFilename.append(it->attribute("placeSound").as_string());
		AXAudioChunk* placeSound = new AXAudioChunk(placeSoundFilename);

		std::string name = it->attribute("name").as_string();
		std::string description = it->attribute("description").as_string();
	    objects.push_back(new Object(id, food, money, wood, requiredType, cost, filename, placeSound, name, description));
	}
}

Tile* World::getMousedTile(){
	return map[mousePosition.x+currentOffset.x][mousePosition.y+currentOffset.y];
}

void World::inGameTick(){
	currentMoney += moneyIncome;
	currentFood += foodIncome;
	currentWood += woodIncome;
	gui->updateResources();
}