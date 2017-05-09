#include "main.hpp"
int main(int argc, char const *argv[]){
	AXWindow::init(1280, 720, "My Game", AX_DEFAULT, tick, draw);
	//build the player
	player = new Player(AXVector2D(0, 10), AXVector2D(40, 40));
	gameobjects.push_back(player);	
	block = new Block(AXVector2D(100, 0), AXVector2D(40, 40));
	gameobjects.push_back(block);	
	return AXWindow::run();
}

void tick(){
	for (auto & go : gameobjects){
		go->physics();
	}
	for (auto & go : gameobjects){
		go->tick();
	}
	//collide check
	for (auto & go1 : gameobjects){
		for (auto & go2 : gameobjects){
			if(go1!=go2){
				while(collideCheck(go1, go2)){
					std::cout << "Collided" << std::endl;
				}
			}
		}
	}
}

void draw(){
	for (auto & go : gameobjects){
		go->draw();
	}
}

bool collideCheck(GameObject* go1, GameObject* go2){
	std::cout << "called" << std::endl;
	AXVector2D& go1pos = go1->getPos();
	AXVector2D& go1size = go1->getSize();
	AXVector2D& go2pos = go2->getPos();
	AXVector2D& go2size = go2->getSize();
	std::cout << go2size.x << std::endl;
	float w = 0.5 * (go1size.x + go2size.x);
	float h = 0.5 * (go1size.y + go2size.y);
	float deltax = (go1pos.x+(go1size.x/2)) - (go2pos.x+(go2size.x/2));
	float deltay = (go1pos.y+(go1size.y/2)) - (go2pos.y+(go2size.y/2));
	if (AXMath::absolute(deltax) < w && AXMath::absolute(deltay) < h)
	{
		std::cout << "Any collision" << std::endl;
	    //by evaluating the 4 quads that the entitiy could have been in before the movement, we can work out where the collision occured.
	    if((go1pos.y+go1size.y) <= go2pos.y){
	        // at the top
	        std::cout << "top collision" << std::endl;
	        go1->setPos(go1pos.x, go2pos.y-go1size.y);
	        return true;
	    }
	    if((go1pos.y) >= go2pos.y+go2size.y){
	        // collision at the bottom
	        std::cout << "bottom collision" << std::endl;
	        go1->setPos(go1pos.x, go2pos.y+go2size.y);
	        return true;
	    }
	    if((go1pos.x+go1size.x) <= go2pos.x){
	        // on the left
	        std::cout << "left collision" << std::endl;
	        go1->setPos(go2pos.x-go1size.x, go1pos.y);
	        return true;
	    }
	    if(go1pos.x>= go2pos.x+go2size.x){
	        // on the right
	        std::cout << "right collision" << std::endl;
	        go1->setPos(go2pos.x+go2size.x, go1pos.y);
	        return true;
	    }
	}
	return false;
}