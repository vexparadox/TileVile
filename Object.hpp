#ifndef Object_hpp
#define Object_hpp
#include <Axilya/AXMain.h>
class Object{
public:
	int id, money, food, wood, stone, pop, requiredType;
	int upgradeID;
	int moneyCost, woodCost, stoneCost;
	AXTexture* texture;
	std::shared_ptr<AXAudioChunk> placeSound; // played when placed
	std::string description, name;
	Object(AXXMLnode_iterator& it);
	Object(const Object* other);
};
#endif