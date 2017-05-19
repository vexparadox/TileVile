#include "World.hpp"

World::World(int width, int height, int tilesize) : width(width), height(height), tilesize(tilesize){
	currentCenter.x = 2;
	currentCenter.y = 2;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1,4);
	for(int i = 0; i < width; i++){
		std::vector<Tile*> vec;
		tiles.push_back(vec);
		for(int j = 0; j < height; j++){
			tiles[i].push_back(new Tile(distribution(generator), tilesize));
		}
	}
}

void World::draw(){
	int row = 0;
	for(int i = currentCenter.x; i < (AXWindow::getWidth()/tilesize)+currentCenter.x; i++){
		int col = 0;
		for(int j = currentCenter.y; j < (AXWindow::getHeight()/tilesize)+currentCenter.y; j++){
			tiles[i][j]->draw((row*tilesize), (col*tilesize));
			col++;
		}
		row++;
	}
	std::cout << "X: " << currentCenter.x << " Y: " << currentCenter.y << std::endl;
}

void World::tick(){

}