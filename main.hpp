#pragma once
#include <Axilya/AXMain.h>
#include "Player.hpp"
#include "Block.hpp"
std::vector<GameObject*> gameobjects;
void tick();
void draw();
bool collideCheck(GameObject* go1, GameObject* go2);
Player* player;
Block* block;