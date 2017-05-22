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
			if(AXInput::getValue("MB1")){
				//set the selected object to the one we clicked on
				world->selectedObject = objectID;
				//rebake the instruction text
				instructionText = fontBig->bakeTexture("Click on a tile to place "+world->objects[objectID]->name+"!", blackColour);
				detailText1 = fontSmall->bakeTexture("This building requires: "+types[world->objects[objectID]->requiredType], blackColour);
				detailText2 = fontSmall->bakeTexture("Cost: $"+std::to_string(world->objects[objectID]->cost)+" | Food: "+std::to_string(world->objects[objectID]->food)+" | Money: "+std::to_string(world->objects[objectID]->money)+" | Wood: "+std::to_string(world->objects[objectID]->wood), blackColour);
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
	AXGraphics::drawTexture(moneyText, AXWindow::getWidth()-moneyText->getWidth()-135, AXWindow::getHeight()-moneyText->getHeight()-80); 
	//the food logo
	AXGraphics::drawTexture(foodIcon, AXWindow::getWidth()-foodText->getWidth()-45, AXWindow::getHeight()-foodText->getHeight()-75, 24, 24); 
	//the food text
	AXGraphics::drawTexture(foodText, AXWindow::getWidth()-foodText->getWidth()-20, AXWindow::getHeight()-foodText->getHeight()-80); 
	//the wood icon
	AXGraphics::drawTexture(woodIcon, AXWindow::getWidth()-woodText->getWidth()-60, AXWindow::getHeight()-woodText->getHeight()-40, 48, 48); 
	//the wood text
	AXGraphics::drawTexture(woodText, AXWindow::getWidth()-woodText->getWidth()-20, AXWindow::getHeight()-woodText->getHeight()-30); 

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

}