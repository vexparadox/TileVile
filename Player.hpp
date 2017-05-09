#ifndef Player_hpp
#define Player_hpp
#include "GameObject.hpp"
class Player : public GameObject{
public:
	Player(AXVector2D position, AXVector2D size);
	void draw() override;
	void tick() override;
};
#endif