#include "GUI.hpp"
#include "World.hpp"
GUI::GUI(World* world) : world(world){
	//load in the two fonts
	fontBig = new AXFont("font/Ayuthaya.ttf", 22);
	fontSmall = new AXFont("font/Ayuthaya.ttf", 14);
	//load pickup sound
	pickupSound = new AXAudioChunk("audio/pickupsound.wav");
	cancelPickupSound = new AXAudioChunk("audio/cancelpickupsound.wav");
	//destruction sond
	destructionSound = new AXAudioChunk("audio/cancelpickupsound.wav");
	//black colour for text rendering
	blackColour = AXColour(0, 0, 0);
	//red colour
	redColour = AXColour(255, 0, 0);
	//the types, this shouldn't really be hard coded
	types.push_back("Flat Land");
	types.push_back("Wooded Area");
	types.push_back("Stone Outcrop");

	townsizes.push_back("A small hamlet");
	townsizes.push_back("A humble village");
	townsizes.push_back("A worthy town");
	townsizes.push_back("A growing city");

	//the townsize text
	townNameText = nullptr;

	//this text is filled when the tile moused over is changed
	//if either shows the description of the tile or its object
	descriptionText = nullptr;

	//these texts describe 
	instructionText = nullptr;
	detailText1 = nullptr; // describes type requirements
	detailText2 = nullptr; // describes cost
	detailText3 = nullptr; // describes production

	//used to remember the last tile moused over, it stops the text rebaking every frame
	lastTileHovered = nullptr;
	//used to remember the last tile selected
	lastTileSelected = nullptr;

	moneyText = nullptr;
	foodText = nullptr;
	woodText = nullptr; 
	stoneText = nullptr;
	popText = nullptr;
	
	//load the resource icons
	foodIcon = new AXTexture("images/icons/meat.png");
	woodIcon = new AXTexture("images/icons/wood.png");
	stoneIcon = new AXTexture("images/icons/stone.png");
	popIcon = new AXTexture("images/icons/population.png");
	//used when deleting
	deleteIcon = new AXTexture("images/icons/delete.png");
	//the background to the gui
	backgroundIMG = new AXTexture("images/guibackground.png");
	updateResources();
	bakeTownText();
	bakeObjectInfoStrings(world->selectedObject, true);
}

void GUI::tick(Tile* tile){
	onGUI = isMouseOverGUI();

	//let the user cancel their pickup
	if(AXInput::getValue("ESC") && world->homeSet && world->selectedObject >= 0){
		world->selectedObject = -1;
		AXAudio::playAudioChunk(cancelPickupSound);
	}

	//let the user cancel their selected tile
	if(AXInput::getValue("ESC") && world->selectedTile){
		world->selectedTile = nullptr;
		AXAudio::playAudioChunk(cancelPickupSound);
	}

	//do the object selection with numbers
	if(world->selectedObject < 0){
		for(int i = 1; i < world->objects.size(); i++){
			//get the input on the numbers
			if(AXInput::getValue(std::to_string(i))){
				world->selectedObject = i;
				bakeObjectInfoStrings(i, true); // bake the info strings
				AXAudio::playAudioChunk(pickupSound); // play the pickup sound
				break;
			}
		}
	}

	// if the tilebeing hovered over has changed
	// rebake the strings
	if(tile != lastTileHovered && !onGUI){
		lastTileHovered = tile;
		//if the tile has an object on it, show that description instead
		delete descriptionText;
		if(tile->object){
			descriptionText = fontSmall->bakeTexture(tile->object->description, blackColour);	
		}else{
			descriptionText = fontSmall->bakeTexture(tile->description, blackColour);	
		}
	}

	//if the tile selected has changed
	//only can be selected
	if(world->selectedTile != lastTileSelected){
		lastTileSelected = world->selectedTile;
		//if it's changing to a tile, not a null
		if(lastTileSelected){
			bakeObjectInfoStrings(lastTileSelected->object->id, false);
		}
	}

	//if there's a tile selected, and they click the bit X delete button
	if(lastTileSelected){
		if(AXInput::getValue("MB1") && AXMath::isInsideQuad(AXInput::mouseX, AXInput::mouseY, AXWindow::getWidth()/2-32, AXWindow::getHeight()-80, AXWindow::getWidth()/2+32, AXWindow::getHeight()-16)){
			world->deleteObject();
		}
	}

	//if the user has nothing selected
	if(world->selectedObject < 0 && onGUI){
		//if they are moused over something and the mouse is pressed!
		int objectID = whichObjectMousedOver();
		if(objectID >= 0){
			if(objectID != lastObjectID){	
				delete descriptionText;
				//show the description of the object
				descriptionText = fontSmall->bakeTexture(world->objects[objectID]->description, blackColour);	
			}
			//if they click on an object
			if(AXInput::getValue("MB1")){
				//set the selected object to the one we clicked on
				world->selectedObject = objectID; 
				bakeObjectInfoStrings(objectID, true); // bake the info strings
				AXAudio::playAudioChunk(pickupSound); // play the pickup sound
			}
		}
		lastObjectID = objectID;
	}
}

void GUI::draw(){
	//draw the background image
	AXGraphics::drawTexture(backgroundIMG, 0, AXWindow::getHeight()-(2*world->tilesize), AXWindow::getWidth(), 2*world->tilesize);


	//if you're placing an object or have a tile selected
	//these strings are baked in bakeObjectInfoStrings
	if(world->selectedObject >= 0 || world->selectedTile){
		//the instruction text tells em
		AXGraphics::drawTexture(instructionText, 20, AXWindow::getHeight()-instructionText->getHeight()-80); 
		if(detailText1 && detailText2){
			AXGraphics::drawTexture(detailText1, 20, AXWindow::getHeight()-detailText1->getHeight()-55); 
			AXGraphics::drawTexture(detailText2, 20, AXWindow::getHeight()-detailText2->getHeight()-32); 
			AXGraphics::drawTexture(detailText3, 20, AXWindow::getHeight()-detailText3->getHeight()-12); 
		}
	}

	//show the delete symbol if you can delete and an object is selected
	if(world->selectedTile){
		//you can't delete the townhall
		if(world->selectedTile->object->id != 0){
			AXGraphics::drawTexture(deleteIcon, AXWindow::getWidth()/2-32, AXWindow::getHeight()-80, 64, 64); 
		}
	}

	//if there's no object or tile selected show the object selection
	if(world->selectedObject < 0 && !world->selectedTile){
		drawObjectSelect();
	}

	//show the town name text
	if(townNameText){
		AXGraphics::drawTexture(townNameText, (AXWindow::getWidth()/2)-(townNameText->getWidth()/2), AXWindow::getHeight()-townNameText->getHeight()-45); 	
	}
	//show the description text
	if(descriptionText){
		AXGraphics::drawTexture(descriptionText, AXWindow::getWidth()-descriptionText->getWidth()-20, AXWindow::getHeight()-descriptionText->getHeight()-10); 
	}
	//show the resources
	//The money text
	AXGraphics::drawTexture(moneyText, AXWindow::getWidth()-moneyText->getWidth()-foodText->getWidth()-60, AXWindow::getHeight()-moneyText->getHeight()-80); 
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
	//the stone icon
	AXGraphics::drawTexture(popIcon, AXWindow::getWidth()-popText->getWidth()-stoneText->getWidth()-woodText->getWidth()-140, AXWindow::getHeight()-woodText->getHeight()-45, 48, 48); 
	//the stone text
	AXGraphics::drawTexture(popText, AXWindow::getWidth()-popText->getWidth()-stoneText->getWidth()-woodText->getWidth()-100, AXWindow::getHeight()-woodText->getHeight()-35); 
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
	delete moneyText;
	if(world->moneyIncome < 0){
		moneyText = fontBig->bakeTexture("$"+std::to_string(world->currentMoney)+"("+std::to_string(world->moneyIncome)+")", redColour);
	}else{
		moneyText = fontBig->bakeTexture("$"+std::to_string(world->currentMoney)+"(+"+std::to_string(world->moneyIncome)+")", blackColour);
	}
	delete foodText;
	if(world->foodIncome < 0){
		foodText = fontBig->bakeTexture(std::to_string(world->currentFood)+"("+std::to_string(world->foodIncome)+")", redColour);
	}else{
		foodText = fontBig->bakeTexture(std::to_string(world->currentFood)+"(+"+std::to_string(world->foodIncome)+")", blackColour);
	}
	delete woodText;
	if(world->woodIncome < 0){
		woodText = fontBig->bakeTexture(std::to_string(world->currentWood)+"("+std::to_string(world->woodIncome)+")", redColour);
	}else{
		woodText = fontBig->bakeTexture(std::to_string(world->currentWood)+"(+"+std::to_string(world->woodIncome)+")", blackColour);
	}
	delete stoneText;
	if(world->stoneIncome < 0){
		stoneText = fontBig->bakeTexture(std::to_string(world->currentStone)+"("+std::to_string(world->stoneIncome)+")", redColour);
	}else{
		stoneText = fontBig->bakeTexture(std::to_string(world->currentStone)+"(+"+std::to_string(world->stoneIncome)+")", blackColour);
	}
	delete popText;
	//if there's not enough food, show that people die
	if(world->currentFood < 0){
		popText = fontBig->bakeTexture(std::to_string(world->currentPop)+"(-2)", redColour);
	}else{
		popText = fontBig->bakeTexture(std::to_string(world->currentPop), blackColour);
	}
}


void GUI::bakeObjectInfoStrings(int objectID, bool placing){
	if(objectID < 0){
		AXLog::log("Bake Object Info", "You're trying to bake info strings when there's no object selected.", AX_LOG_ERROR);
		return;
	}
	delete instructionText;
	delete detailText1;
	delete detailText2;
	delete detailText3;
	Object* selected = world->objects[objectID]; // get a temp object
	//if you're placing, give an instruction, otherwise give a statement
	if(placing){
		instructionText = fontBig->bakeTexture("Click to place a "+selected->name+"!", blackColour);
	}else{
		instructionText = fontBig->bakeTexture("A "+selected->name+"!", blackColour);
	}
	//bake the detail1 string, it will say the type it requires or how to delete
	if(placing){
		detailText1 = fontSmall->bakeTexture("Tile Type: "+types[selected->requiredType], blackColour);
	}else{
		//YOU CANT DELETE THE TOWN HALL
		if(objectID == 0){
			detailText1 = fontSmall->bakeTexture("This building gives you "+std::to_string(world->allowedHomeDistance)+" tiles to build on.", blackColour);
		}else{
			detailText1 = fontSmall->bakeTexture("If you destroy this, you'll get $"+std::to_string((int)selected->cost/2), blackColour);
		}
	}
	//create a temporary string to hold the details
	std::string detailText2String = "Cost | $"+std::to_string(selected->cost);
	detailText2 = fontSmall->bakeTexture(detailText2String, blackColour);
	//a string for the production
	std::string detailText3String = "Per Tick";
	if(selected->money != 0){
		detailText3String.append(" | $"+std::to_string(selected->money));
	}
	if(selected->food != 0){
		detailText3String.append(" | Food: "+std::to_string(selected->food));
	}
	if(selected->pop != 0){
		detailText3String.append(" | Population: "+std::to_string(selected->pop));
	}
	if(selected->wood != 0){
		detailText3String.append(" | Wood: "+std::to_string(selected->wood));
	}
	if(selected->stone != 0){
		detailText3String.append(" | Stone: "+std::to_string(selected->stone));
	}
	detailText3 = fontSmall->bakeTexture(detailText3String, blackColour);
}

void GUI::bakeTownText(){
	delete townNameText;
	townNameText = fontBig->bakeTexture(townsizes[world->townSize], blackColour);
}

GUI::~GUI(){
	//delete all the shit
	delete backgroundIMG;
	delete instructionText;
	delete detailText1;
	delete detailText2;
	delete detailText3;
	delete descriptionText;
	delete townNameText;
	delete moneyText;
	delete foodText;
	delete foodIcon;
	delete woodText;
	delete woodIcon;
	delete stoneText;
	delete stoneIcon;
	delete deleteIcon;
	delete popText;
	delete popIcon;
	delete fontBig;
	delete fontSmall;
	delete pickupSound;
	delete cancelPickupSound;
}