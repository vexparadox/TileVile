#ifndef GameObject_hpp
#define GameObject_hpp
#include <Axilya/AXMain.h>
#include <Box2D/Box2D.h>
extern b2World* world;
class GameObject{
protected:
	AXVector2D position;
	AXVector2D size;
	AXVector2D velocity; //added on tick
	bool gravity;
	AXResourceManager* rm = AXResourceManager::getInstance();
	virtual ~GameObject(){};
	GameObject(AXVector2D position, AXVector2D size, bool gravity);
public:
	virtual void draw() = 0;
	virtual void tick() = 0;

	void physics();

	AXVector2D& getPos();
	AXVector2D& getSize();
	void setPos(float x, float y);
	void setSize(float x, float y);
};
#endif