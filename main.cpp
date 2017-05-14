#include "main.hpp"
// NOTE OT WILL
// All Polygon Shapes objects have a 1.0f skin around them
//define the world
b2World* world;
b2Body* body;
b2Body* groundBody;
int main(int argc, char const *argv[]){
	AXWindow::init(1280, 720, "My Game", AX_DEFAULT, tick, draw);

	//init the world
	world = new b2World(b2Vec2(0.0f, 10.0f));
	//build the player
	b2BodyDef groundDefinitions;
	groundDefinitions.position.Set(20.0f, 30.0f);
	//create the body
	groundBody = world->CreateBody(&groundDefinitions);
	//size of the body
	b2PolygonShape groundBox;
	groundBox.SetAsBox(10.0f, 10.0f);
	//add fixture
	groundBody->CreateFixture(&groundBox, 0.0f);


	//Now create dynamic
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.fixedRotation = true;
	bodyDef.position.Set(10.0f, 10.0f);
	body = world->CreateBody(&bodyDef);

	//give it a shape
	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox(1.0f, 1.0f);
	//create ficture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 2.0f;
	fixtureDef.friction = 0.5f;
	body->CreateFixture(&fixtureDef);
	int ret = AXWindow::run();
	delete world;
	return ret;
}

void tick(){
	float timeStep = 1.0f/60.0f;
	int velocityIterations = 6; 
	int positionIterations = 2;
	world->Step(timeStep, velocityIterations, positionIterations);
	if(AXInput::getValue("D")){
		const b2Vec2 force(200, 0); 
		body->ApplyForce(force, body->GetWorldCenter(), true);
	}
	if(AXInput::getValue("A")){
		const b2Vec2 force(-200, 0); 
		body->ApplyForce(force, body->GetWorldCenter(), true);
	}
	if(AXInput::getValue("W")){
		const b2Vec2 force(0, -200); 
		body->ApplyForce(force, body->GetWorldCenter(), true);
	}
	if(AXInput::getValue("S")){
		const b2Vec2 force(0, 200); 
		body->ApplyForce(force, body->GetWorldCenter(), true);
	}
	if(AXInput::getValue("SPACE")){
		world->SetGravity(b2Vec2(0, 0));
	}
}

void draw(){
	AXGraphics::fill(0, 0, 0);
	AXGraphics::drawRect((body->GetPosition().x*10)-10, (body->GetPosition().y*10)-10, 20, 20);
	AXGraphics::fill(255, 0, 0);
	AXGraphics::drawRect((groundBody->GetPosition().x*10)-100, (groundBody->GetPosition().y*10)-100, 200, 200);
	AXGraphics::fill(0, 255, 0);
	AXGraphics::drawPoint(AXVector2D(groundBody->GetPosition().x, groundBody->GetPosition().y));
	for (auto & go : gameobjects){
		go->draw();
	}
}