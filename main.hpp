#pragma once
#include <Axilya/AXMain.h>
#include <Box2D/Box2D.h>
#include "Player.hpp"
#include "Block.hpp"
std::vector<GameObject*> gameobjects;
extern b2World* world;
void tick();
void draw();
bool collideCheck(GameObject* go1, GameObject* go2);
Player* player;
Block* block;