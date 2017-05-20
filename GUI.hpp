#pragma once
#include <Axilya/AXMain.h>
class World;
class GUI {
	AXTexture* positionText;
	AXFont* font;
	World* world;
public:
	GUI(World* world);
	void draw();
	void tick();
	void updatePosition(const AXVector2D& offset);
};