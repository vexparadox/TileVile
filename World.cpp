#include "World.hpp"

World::World(int width, int height, int tilesize) : width(width), height(height), tilesize(tilesize){


	left = false;
	right = false;
	up = false;
	down = false;
	//set how many can be on screen at once
	maxOnScreenX = AXWindow::getWidth()/tilesize;
	maxOnScreenY = AXWindow::getHeight()/tilesize;
	if(height < maxOnScreenY || width < maxOnScreenX){
		AXLog::log("World", "You can't make a map that's smaller than the screen.", AX_LOG_ERROR);
	}
	//generate the tiles
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,4);
	for(int i = 0; i < width; i++){
		std::vector<Tile*> vec;
		tiles.push_back(vec);
		for(int j = 0; j < height; j++){
			tiles[i].push_back(new Tile(distribution(generator)));
		}
	}
}

void World::draw(){
	int row = 0;
	//this loop will go from the offset to the maxonScreen+offset, meaning it draws the visible porition of the world
	for(int i = currentOffset.x; i < maxOnScreenX+currentOffset.x; i++){
		int col = 0;
		for(int j = currentOffset.y; j < maxOnScreenY+currentOffset.y; j++){
			//call a fill colour switch
			if(row == playerPosition.x && col == playerPosition.y){
				switchTileColour(0);
			}else{
				switchTileColour(tiles[i][j]->id);				
			}
			AXGraphics::drawRect((row*tilesize), (col*tilesize), tilesize, tilesize);
			col++; // advance the col draw position
		}
		row++; // advance the row draw position
	}
}

void World::tick(){
	//move the player, it will be corrected later
	if(AXInput::getValue("LEFT") && !left){
		left = true;
		if(playerPosition.x != 0){
			playerPosition.x--;
		}
	}else if(!AXInput::getValue("LEFT") && left){
		left = false;
	}

	if(AXInput::getValue("RIGHT") && !right){
		right = true;
		if(playerPosition.x+1 < maxOnScreenX){
			playerPosition.x++;
		}
	}else if(!AXInput::getValue("RIGHT") && right){
		right = false;
	}


	if(AXInput::getValue("UP") && !up){
		up = true;
		if(playerPosition.y != 0){
			playerPosition.y--;
		}
	}else if(!AXInput::getValue("UP") && up){
		up = false;
	}

	if(AXInput::getValue("DOWN") && !down){
		down = true;
		if(playerPosition.y+1 < maxOnScreenY){
			playerPosition.y++;
		}
	}else if(!AXInput::getValue("DOWN") && down){
		down = false;
	}
	//this will move the world!
	
	//if the player is past the middle but not at the edge of the world
	if(playerPosition.x > maxOnScreenX/2 && maxOnScreenX+currentOffset.x < width){
		currentOffset.x++;
		playerPosition.x = maxOnScreenX/2;
	}

	//if the player is going going left but not at the edge of the world
	if(playerPosition.x < maxOnScreenX/2 && currentOffset.x != 0){
		currentOffset.x--;
		playerPosition.x = maxOnScreenX/2;
	}


	if(playerPosition.y > maxOnScreenY/2 && maxOnScreenY+currentOffset.y < height){
		currentOffset.y++;
		playerPosition.y = maxOnScreenY/2;
	}


	if(playerPosition.y < maxOnScreenY/2 && currentOffset.y != 0){
		currentOffset.y--;
		playerPosition.y = maxOnScreenY/2;
	}
}

void World::switchTileColour(int id){
	switch(id){
		case 0:
			AXGraphics::fill(255, 0, 0);
		break;
		case 1:
			AXGraphics::fill(0, 255, 0);
		break;
		case 2:
			AXGraphics::fill(0, 255, 255);
		break;
		case 3:
			AXGraphics::fill(20, 155, 255);
		break;
		case 4:
			AXGraphics::fill(0, 50, 60);
		break;
	}

}