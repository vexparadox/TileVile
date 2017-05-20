#include "World.hpp"

World::~World(){
	delete playerTexture;
	delete gui;
}

World::World(int tilesize) : tilesize(tilesize){
	//input booleans, allows for stepping
	left = false;
	right = false;
	up = false;
	down = false;

	//create the GUI
	gui = new GUI();
	gui->updatePosition(currentOffset+playerPosition);

	//set how many can be on screen at once
	maxOnScreenX = AXWindow::getWidth()/tilesize;
	maxOnScreenY = (AXWindow::getHeight()/tilesize)-2;

	
	//loads world textures, not tiles
	loadTextures();
	//load tiles in
	loadTiles();

	//load in the map and genertae
	loadMap();

	if(height < maxOnScreenY || width < maxOnScreenX){
		AXLog::log("World", "You can't make a map that's smaller than the screen.", AX_LOG_ERROR);
	}
}

void World::draw(){
	int row = 0;
	//this loop will go from the offset to the maxonScreen+offset, meaning it draws the visible porition of the world
	for(int i = currentOffset.x; i < maxOnScreenX+currentOffset.x; i++){
		int col = 0;
		for(int j = currentOffset.y; j < maxOnScreenY+currentOffset.y; j++){
			//draw the background
			AXGraphics::drawTexture(map[i][j]->texture, (row*tilesize), (col*tilesize), tilesize, tilesize);
			col++; // advance the col draw position
		}
		row++; // advance the row draw position
	}
	//draw the player
	AXGraphics::drawTexture(playerTexture, (playerPosition.x*tilesize), (playerPosition.y*tilesize), tilesize, tilesize);
	//draw the gui
	gui->draw();
}

void World::tick(){
	// move the player, it will be corrected later
	// this uses step movement
	if(AXInput::getValue("LEFT") && !left){
		left = true;
		if(playerPosition.x != 0 && playerEnergy > 0){
			playerPosition.x--;
			gui->updatePosition(currentOffset+playerPosition);
			playerEnergy--;
		}
	}else if(!AXInput::getValue("LEFT") && left){
		left = false;
	}

	if(AXInput::getValue("RIGHT") && !right){
		right = true;
		if(playerPosition.x+1 < maxOnScreenX && playerEnergy > 0){
			playerPosition.x++;
			gui->updatePosition(currentOffset+playerPosition);
			playerEnergy--;
		}
	}else if(!AXInput::getValue("RIGHT") && right){
		right = false;
	}


	if(AXInput::getValue("UP") && !up){
		up = true;
		if(playerPosition.y != 0 && playerEnergy > 0){
			playerPosition.y--;
			gui->updatePosition(currentOffset+playerPosition);
			playerEnergy--;
		}
	}else if(!AXInput::getValue("UP") && up){
		up = false;
	}

	if(AXInput::getValue("DOWN") && !down){
		down = true;
		if(playerPosition.y+1 < maxOnScreenY && playerEnergy > 0){
			playerPosition.y++;
			gui->updatePosition(currentOffset+playerPosition);
			playerEnergy--;
		}
	}else if(!AXInput::getValue("DOWN") && down){
		down = false;
	}
	//this will move the world!
	
	//how much movement the player has in the middle of the screen
	int xAllowance = 2;
	int yAllowance = 2;
	//if the player is past the middle but not at the edge of the world
	if(playerPosition.x > maxOnScreenX/2+xAllowance && maxOnScreenX+currentOffset.x < width){
		currentOffset.x++;
		playerPosition.x = maxOnScreenX/2+xAllowance;
	}

	//if the player is going going left but not at the edge of the world
	if(playerPosition.x < maxOnScreenX/2-xAllowance && currentOffset.x != 0){
		currentOffset.x--;
		playerPosition.x = maxOnScreenX/2-xAllowance;
	}


	if(playerPosition.y > maxOnScreenY/2+yAllowance && maxOnScreenY+currentOffset.y < height){
		currentOffset.y++;
		playerPosition.y = maxOnScreenY/2+yAllowance;
	}


	if(playerPosition.y < maxOnScreenY/2-yAllowance && currentOffset.y != 0){
		currentOffset.y--;
		playerPosition.y = maxOnScreenY/2-yAllowance;
	}
	//update the gui
	gui->tick();
}
void World::loadTextures(){
	//load the player
	this->playerTexture = new AXTexture("images/man1.png");
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
		std::string filename = it->attribute("filename").as_string();
	    tiles.push_back(new Tile(id, filename));
	}
}