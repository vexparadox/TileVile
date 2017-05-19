#pragma once
#include <Axilya/AXMain.h>
class Tile{
public:
	int id;
	int size;
	Tile(int id, int size);
	void draw(int x, int y);
};