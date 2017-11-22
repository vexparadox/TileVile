#ifndef Object_hpp
#define Object_hpp
#include <Axilya/AXMain.h>
#include "ResourcePool.hpp"
class Object{
public:
	int id, requiredType;
	int upgradeID;
	ResourceCost cost;
	ResourceIncome income;
	AXTexture* texture;
	std::shared_ptr<AXAudioChunk> placeSound; // played when placed
	std::string description, name;
	Object(AXXMLnode_iterator& it);
	Object(const Object* other);

	bool HasCost();
};
#endif