#include "GUI.hpp"
#include "World.hpp"
GUI::GUI(World* world) : world(world){
	//load in the two fonts
	fontBig = new AXFont("font/Ayuthaya.ttf", 22);
	fontSmall = new AXFont("font/Ayuthaya.ttf", 14);
	//load pickup sound
	pickupSound = new AXAudioChunk("audio/pickupsound.wav");
	cancelPickupSound = new AXAudioChunk("audio/cancelpickupsound.wav");
	//black colour for text rendering
	blackColour = AXColour(0, 0, 0);
	//the types, this shouldn't really be hard coded
	types.push_back("Flat Land");
	types.push_back("Wooded Area");
	types.push_back("Stone Outcrop");
	//the initial instructions
	instructionText = fontBig->bakeTexture("Click on a tile to place your town hall!", blackColour);
	//this text is the same
	cantPlaceText = fontSmall->bakeTexture("You can't place that here.", blackColour);

	//this text is filled when the tile moused over is changed
	//if either shows the description of the tile or its object
	descriptionText = nullptr;
	detailText1 = nullptr;
	detailText2 = nullptr;

	//used to remember the last tile moused over, it stops the text rebaking every frame
	lastTile = nullptr;
	
	//load the resource icons
	foodIcon = new AXTexture("images/icons/meat.png");
	woodIcon = new AXTexture("images/icons/wood.png");
	stoneIcon = new AXTexture("images/icons/stone.png");
	backgroundIMG = new AXTexture("images/guibackground.png");
	updateResources();
}

void GUI::tick(Tile* tile){
	onGUI = isMouseOverGUI();

	//let the user cancel their pickup
	if(AXInput::getValue("ESC") && world->homeSet && world->selectedObject >= 0){
		world->selectedObject = -1;
		AXAudio::playAudioChunk(cancelPickupSound);
	}

	// if the tile has changed
	// rebake the strings
	if(tile != lastTile && !onGUI){
		lastTile = tile;
		//if the tile has an object on it, show that description instead
		if(tile->object){
			descriptionText = fontSmall->bakeTexture(tile->object->description, blackColour);	
		}else{
			descriptionText = fontSmall->bakeTexture(tile->description, blackColour);	
		}
	}

	//if the user has nothing selected
	if(world->selectedObject < 0 && onGUI){
		//if they are moused over something and the mouse is pressed!
		int objectID = whichObjectMousedOver();
		if(objectID >= 0){
			if(objectID != lastObjectID){		
				descriptionText = fontSmall->bakeTexture(world->objects[objectID]->description+" : $"+std::to_string(world->objects[objectID]->cost), blackColour);	
			}
			//if they click on an object
			if(AXInput::getValue("MB1")){
				//set the selected object to the one we clicked on
				world->selectedObject = objectID; 
				Object* selected = world->objects[objectID]; // get a temp object
				//rebake the instruction text
				instructionText = fontBig->bakeTexture("Click on a tile to place "+selected->name+"!", blackColour);
				//bake the detail1 string, it will say the type it requires
				detailText1 = fontSmall->bakeTexture("This building requires: "+types[selected->requiredType], blackColour);
				//create a temporary string to hold the details
				std::string detailText2String = "Cost: $"+std::to_string(selected->cost);
				if(selected->food != 0){
					detailText2String.append(" | Food: "+std::to_string(selected->food));
				}
				if(selected->money != 0){
					detailText2String.append(" | Money: "+std::to_string(selected->money));
				}
				if(selected->wood != 0){
					detailText2String.append(" | Wood: "+std::to_string(selected->wood));
				}
				if(selected->stone != 0){
					detailText2String.append(" | Stone: "+std::to_string(selected->stone));
				}
				detailText2 = fontSmall->bakeTexture(detailText2String, blackColour);
				AXAudio::playAudioChunk(pickupSound);
			}
		}
		lastObjectID = objectID;
	}
	
}

void GUI::draw(){
	AXGraphics::drawTexture(backgroundIMG, 0, AXWindow::getHeight()-(2*world->tilesize), AXWindow::getWidth(), 2*world->tilesize);
	//if there's an object to place
	if(world->selectedObject >= 0){
		//the instruction text tells em
		AXGraphics::drawTexture(instructionText, 20, AXWindow::getHeight()-instructionText->getHeight()-80); 
		if(detailText1 && detailText2){
			AXGraphics::drawTexture(detailText1, 20, AXWindow::getHeight()-detailText1->getHeight()-50); 
			AXGraphics::drawTexture(detailText2, 20, AXWindow::getHeight()-detailText2->getHeight()-32); 
		}
	}
	//if the last tile isn't placeable and there's an object waiting to be placed
	if(lastTile && world->selectedObject >= 0){
		if(lastTile->object || world->objects[world->selectedObject]->requiredType != lastTile->type){
			AXGraphics::drawTexture(cantPlaceText, 20, AXWindow::getHeight()-instructionText->getHeight()-5); 
		}
	}
	//if there's no object selected let them pick one
	if(world->selectedObject < 0){
		drawObjectSelect();
	}
	//show the description text
	if(descriptionText){
		AXGraphics::drawTexture(descriptionText, AXWindow::getWidth()-descriptionText->getWidth()-20, AXWindow::getHeight()-descriptionText->getHeight()-10); 
	}

	//show the resources
	//The money text
	AXGraphics::drawTexture(moneyText, AXWindow::getWidth()-moneyText->getWidth()-foodText->getWidth()-70, AXWindow::getHeight()-moneyText->getHeight()-80); 
	//the food logo
	AXGraphics::drawTexture(foodIcon, AXWindow::getWidth()-foodText->getWidth()-45, AXWindow::getHeight()-foodText->getHeight()-75, 24, 24); 
	//the food text
	AXGraphics::drawTexture(foodText, AXWindow::getWidth()-foodText->getWidth()-20, AXWindow::getHeight()-foodText->getHeight()-80); 
	//the wood icon
	AXGraphics::drawTexture(woodIcon, AXWindow::getWidth()-woodText->getWidth()-60, AXWindow::getHeight()-woodText->getHeight()-45, 48, 48); 
	//the wood text
	AXGraphics::drawTexture(woodText, AXWindow::getWidth()-woodText->getWidth()-20, AXWindow::getHeight()-woodText->getHeight()-35); 
	//the stone icon
	AXGraphics::drawTexture(stoneIcon, AXWindow::getWidth()-stoneText->getWidth()-woodText->getWidth()-100, AXWindow::getHeight()-woodText->getHeight()-45, 48, 48); 
	//the stone text
	AXGraphics::drawTexture(stoneText, AXWindow::getWidth()-stoneText->getWidth()-woodText->getWidth()-60, AXWindow::getHeight()-woodText->getHeight()-35); 


}

int GUI::whichObjectMousedOver(){
	int row = 0;
	int col = -1; // starts by being incrememted
	int numPerRow = 6; // it's actually 1 less than this
	//start from one, because they can't place another town center
	for(int i = 1; i < world->objects.size(); i++){
		//increase the row count for every 4 objects
		if(i%numPerRow == 0){
			row++;
		}
		//if it's not on a new row, incrememnt, else set back to 0
		if(i%numPerRow != 0){
			col++;
		}else{
			col = 0;
		}
		//do a simple mouse in box using the dimensions used by drawObjectSelect
		if(AXInput::mouseX > 20+(world->tilesize*col) && AXInput::mouseX < 20+(world->tilesize*col)+world->tilesize){
			if(AXInput::mouseY > AXWindow::getHeight()-(120-(world->tilesize*row)) && AXInput::mouseY < AXWindow::getHeight()-(120-(world->tilesize*row))+world->tilesize){
				return i;
			}
		}
	}
	return -1;

}

void GUI::drawObjectSelect(){
	int row = 0;
	int col = -1; // starts by being incrememted
	int numPerRow = 6; // it's actually 1 less than this
	//start from one, because they can't place another town center
	for(int i = 1; i < world->objects.size(); i++){
		//increase the row count for every 4 objects
		if(i%numPerRow == 0){
			row++;
		}
		//if it's not on a new row, incrememnt, else set back to 0
		if(i%numPerRow != 0){
			col++;
		}else{
			col = 0;
		}
		AXGraphics::drawTexture(world->objects[i]->texture, 20+(world->tilesize*col), AXWindow::getHeight()-(120-(world->tilesize*row)), world->tilesize, world->tilesize); 
	}
}

bool GUI::isMouseOverGUI(){
	if(AXInput::mouseY > world->maxOnScreenY*world->tilesize){
		return true;
	}
	return false;
}
void GUI::updateResources(){
	if(world->moneyIncome < 0){
		moneyText = fontBig->bakeTexture("$"+std::to_string(world->currentMoney)+"("+std::to_string(world->moneyIncome)+")", blackColour);
	}else{
		moneyText = fontBig->bakeTexture("$"+std::to_string(world->currentMoney)+"(+"+std::to_string(world->moneyIncome)+")", blackColour);
	}

	if(world->foodIncome < 0){
		foodText = fontBig->bakeTexture(std::to_string(world->currentFood)+"("+std::to_string(world->foodIncome)+")", blackColour);
	}else{
		foodText = fontBig->bakeTexture(std::to_string(world->currentFood)+"(+"+std::to_string(world->foodIncome)+")", blackColour);
	}

	if(world->woodIncome < 0){
		woodText = fontBig->bakeTexture(std::to_string(world->currentWood)+"("+std::to_string(world->woodIncome)+")", blackColour);
	}else{
		woodText = fontBig->bakeTexture(std::to_string(world->currentWood)+"(+"+std::to_string(world->woodIncome)+")", blackColour);
	}

	if(world->stoneIncome < 0){
		stoneText = fontBig->bakeTexture(std::to_string(world->currentStone)+"("+std::to_string(world->stoneIncome)+")", blackColour);
	}else{
		stoneText = fontBig->bakeTexture(std::to_string(world->currentStone)+"(+"+std::to_string(world->stoneIncome)+")", blackColour);
	}

}

GUI::~GUI(){
	//delete all the shit
	delete backgroundIMG; // background image for the GUI
	delete instructionText; // used to give instructions
	delete detailText1; // used to give instructions
	delete detailText2; // used to give instructions
	delete descriptionText; // used to describe what the user is selecting
	delete cantPlaceText; // says that it can't place
	delete moneyText; // the current cash
	delete foodText; // the current food
	delete foodIcon; // the food icon
	delete woodText; // the current wood
	delete woodIcon; // the wood icon
	delete stoneText; //the current stone
	delete stoneIcon; // the stone icon
	delete fontBig;
	delete fontSmall;
	delete pickupSound;
	delete cancelPickupSound;
}