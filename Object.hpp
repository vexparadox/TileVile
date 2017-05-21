#ifndef Object_hpp
#define Object_hpp
#include <Axilya/AXMain.h>
class Object{
public:
	int id, money, food, cost, requiredType;
	AXTexture* texture;
	std::string description;
	Object(int id, int food, int money, int requiredType, int cost, const std::string& filename, const std::string& description);
	Object(const Object* other);
};
#endif