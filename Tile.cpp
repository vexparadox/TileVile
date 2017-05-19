#include "Tile.hpp"

Tile::Tile(int id, int size) : id(id), size(size){
}

void Tile::draw(int x, int y){
	switch(id){
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
	AXGraphics::drawRect(x, y, size, size);
}