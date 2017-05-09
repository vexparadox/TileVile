#ifndef Block_hpp
#define Block_hpp
#include "GameObject.hpp"
class Block : public GameObject{
public:
	Block(AXVector2D position, AXVector2D size);
	void draw() override;
	void tick() override;
};
#endif