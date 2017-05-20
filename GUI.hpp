#pragma once
#include <Axilya/AXMain.h>
class GUI {
	AXTexture* positionText;
	AXFont* font;
public:
	GUI();
	void draw();
	void tick();
	void updatePosition(const AXVector2D& offset);
};