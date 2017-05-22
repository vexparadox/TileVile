#ifndef Object_hpp
#define Object_hpp
#include <Axilya/AXMain.h>
class Object{
public:
	int id, money, food, wood, stone, cost, requiredType;
	AXTexture* texture;
	AXAudioChunk* placeSound; // played when placed
	std::string description, name;
	Object(int id, int food, int money, int wood, int stone, int requiredType, int cost, const std::string& filename, AXAudioChunk* placeSound, const std::string& name, const std::string& description);
	Object(const Object* other);
};
#endif