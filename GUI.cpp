#include "GUI.hpp"
#include "World.hpp"
#include "ResourcePool.hpp"

namespace{
	std::string name_from_speed_enum(TICK_SPEED speed)
	{
		switch(speed){
			case SLOW:
				return "Relaxing";
			break;
			case MEDIUM:
				return "Casual";
			break;
			case FAST:
				return "Exhilarating";
			break;
		}
	}
}

GUI::GUI(World* world) : world(world){
	//load in the two fonts
	fontBig = std::make_unique<AXFont>("font/Ayuthaya.ttf", 22);
	fontSmall = std::make_unique<AXFont>("font/Ayuthaya.ttf", 14);
	//load pickup sound
	pickupSound = std::make_unique<AXAudioChunk>("audio/pickupsound.wav");
	cancelPickupSound = std::make_unique<AXAudioChunk>("audio/cancelpickupsound.wav");
	//destruction sond
	destructionSound = std::make_unique<AXAudioChunk>("audio/destructionsound.wav");
	//the sound when you select something
	selectionSound = std::make_unique<AXAudioChunk>("audio/selectsound.wav");
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
	//used to remember the last tile moused over, it stops the text rebaking every frame
	lastTileHovered = nullptr;
	//used to remember the last tile selected
	lastTileSelected = nullptr;	
	//load the resource icons
	foodIcon = std::make_unique<AXTexture>("images/icons/meat.png");
	woodIcon = std::make_unique<AXTexture>("images/icons/wood.png");
	stoneIcon = std::make_unique<AXTexture>("images/icons/stone.png");
	popIcon = std::make_unique<AXTexture>("images/icons/population.png");
	//used when deleting
	deleteIcon = std::make_unique<AXTexture>("images/icons/delete.png");
	//the background to the gui
	backgroundIMG = std::make_unique<AXTexture>("images/guibackground.png");
	updateResources();
	bakeTownText();
	bakeSpeedText();
	bakeObjectInfoStrings(world->selectedObject, true);
}

void GUI::tick(Tile* tile){
	onGUI = isMouseOverGUI();

	//let the user cancel their pickup
	if(AXInput::getValue("ESC") && world->homeSet && world->selectedObject){
		world->selectedObject.reset();
		AXAudio::playAudioChunk(cancelPickupSound);
	}

	//let the user cancel their selected tile
	if(AXInput::getValue("ESC") && world->selectedTile){
		world->selectedTile = nullptr;
		AXAudio::playAudioChunk(cancelPickupSound);
	}

	//change speed using the numbers
	for(int i = 1; i < 4; i ++)
	{
		if(world->current_tick_speed != i && AXInput::getValue(std::to_string(i))){
			world->current_tick_speed = TICK_SPEED(i);
			bakeSpeedText();
			break;
		}
	}
	//do the object selection with numbers
	// if(world->homeSet){
	// 	for(int i = 1; i < world->objects.size(); i++){
	// 		//get the input on the numbers
	// 		if(AXInput::getValue(std::to_string(i))){
	// 			if(world->selectedObject != world->objects.at(i)){
	// 				world->selectedObject = world->objects.at(i);
	// 				bakeObjectInfoStrings(world->selectedObject, true); // bake the info strings
	// 				AXAudio::playAudioChunk(pickupSound); // play the pickup sound
	// 			}
	// 			break;
	// 		}
	// 	}
	// }

	// if the tilebeing hovered over has changed
	// rebake the strings
	if(tile != lastTileHovered && !onGUI){
		lastTileHovered = tile;
		//if the tile has an object on it, show that description instead
		if(tile->object){
			descriptionText.reset(fontSmall->bakeTexture(tile->object->description, blackColour));
		}else{
			descriptionText.reset(fontSmall->bakeTexture(tile->description, blackColour));
		}
	}

	//if the tile selected has changed
	//only can be selected
	if(world->selectedTile != lastTileSelected){
		lastTileSelected = world->selectedTile;
		//if it's changing to a tile, not a null
		if(lastTileSelected){
			bakeObjectInfoStrings(lastTileSelected->object, false);
		}
	}

	//if there's a tile selected, and they click the bit X delete button
	if(lastTileSelected){
		if(AXInput::getValue("MB1") && lastTileSelected->object->id != 0 && AXMath::isInsideQuad(AXInput::mouseX, AXInput::mouseY, AXWindow::getWidth()/2-32, AXWindow::getHeight()-80, AXWindow::getWidth()/2+32, AXWindow::getHeight()-16)){
			world->deleteObject();
		}
	}

	//if the user has nothing selected
	if(world->selectedObject == nullptr && onGUI){
		//if they are moused over something and the mouse is pressed!
		std::shared_ptr<Object> objectPtr = whichObjectMousedOver();
		if(objectPtr){
			if(objectPtr != objectPtr){	
				//show the description of the object
				descriptionText.reset(fontSmall->bakeTexture(objectPtr->description, blackColour));
			}
			//if they click on an object
			if(AXInput::getValue("MB1")){
				//set the selected object to the one we clicked on
				world->selectedObject = objectPtr; 
				bakeObjectInfoStrings(objectPtr, true); // bake the info strings
				AXAudio::playAudioChunk(pickupSound); // play the pickup sound
			}
		}
		objectPtr = objectPtr;
	}
}

void GUI::draw(){

	int topYofGUI = AXWindow::getHeight()-(world->guiTileSize*world->tilesize);
	//draw the background image
	AXGraphics::drawTexture(backgroundIMG.get(), 0, topYofGUI, AXWindow::getWidth(), world->maxOnScreenY*world->tilesize);


	//if you're placing an object or have a tile selected
	//these strings are baked in bakeObjectInfoStrings
	if(world->selectedObject || world->selectedTile){
		//the instruction text tells em
		//spacing of 25
		AXGraphics::drawTexture(instructionText, 20, topYofGUI+2); 
		if(detailText1 && detailText2){
			AXGraphics::drawTexture(detailText1, 20, topYofGUI+40); 
			AXGraphics::drawTexture(detailText2, 20, topYofGUI+65); 
			AXGraphics::drawTexture(detailText3, 20, topYofGUI+90); 
		}
	}

	//show the delete symbol if you can delete and an object is selected
	if(world->selectedTile){
		//you can't delete the townhall
		if(world->selectedTile->object->id != 0){
			AXGraphics::drawTexture(deleteIcon, AXWindow::getWidth()/2-32, topYofGUI+20+townNameText->getHeight(), 64, 64); 
		}
	}

	//if there's no object or tile selected show the object selection
	if(world->selectedObject == nullptr && !world->selectedTile){
		drawObjectSelect();
	}

	//show the town name text
	if(townNameText){
		AXGraphics::drawTexture(townNameText, (AXWindow::getWidth()/2)-(townNameText->getWidth()/2), topYofGUI+20); 	
	}

	//show the speed text
	if(worldSpeedText)
	{
		AXGraphics::drawTexture(worldSpeedText, AXWindow::getWidth()-200, topYofGUI+5); 
	}

	//show the description text
	if(descriptionText){
		AXGraphics::drawTexture(descriptionText, AXWindow::getWidth()-descriptionText->getWidth()-10, AXWindow::getHeight()-descriptionText->getHeight()); 
	}
	//show the resources
	//The money text
	AXGraphics::drawTexture(moneyText, AXWindow::getWidth()-moneyText->getWidth()-foodText->getWidth()-60, topYofGUI+30); 
	//the food logo
	AXGraphics::drawTexture(foodIcon, AXWindow::getWidth()-foodText->getWidth()-45, topYofGUI+35, 24, 24); 
	//the food text
	AXGraphics::drawTexture(foodText, AXWindow::getWidth()-foodText->getWidth()-20, topYofGUI+30); 

	//the wood icon
	AXGraphics::drawTexture(woodIcon, AXWindow::getWidth()-woodText->getWidth()-60, topYofGUI+70, 48, 48); 
	//the wood text
	AXGraphics::drawTexture(woodText, AXWindow::getWidth()-woodText->getWidth()-20, topYofGUI+75); 
	//the stone icon
	AXGraphics::drawTexture(stoneIcon, AXWindow::getWidth()-stoneText->getWidth()-woodText->getWidth()-100, topYofGUI+70, 48, 48); 
	//the stone text
	AXGraphics::drawTexture(stoneText, AXWindow::getWidth()-stoneText->getWidth()-woodText->getWidth()-60, topYofGUI+75); 
	//the stone icon
	AXGraphics::drawTexture(popIcon, AXWindow::getWidth()-popText->getWidth()-stoneText->getWidth()-woodText->getWidth()-140, topYofGUI+70, 48, 48); 
	//the stone text
	AXGraphics::drawTexture(popText, AXWindow::getWidth()-popText->getWidth()-stoneText->getWidth()-woodText->getWidth()-100, topYofGUI+75); 
}

std::shared_ptr<Object> GUI::whichObjectMousedOver(){
	int row = 0;
	int col = -1; // starts by being incrememted
	int numPerRow = 6; // it's actually 1 less than this
	int topYofGUI = AXWindow::getHeight()-(world->guiTileSize*world->tilesize);
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
			if(AXInput::mouseY > topYofGUI+(world->tilesize*row)+20 && AXInput::mouseY < topYofGUI+(world->tilesize*row)+20+world->tilesize){
				return world->objects.at(i);
			}
		}
	}
	return nullptr;

}

void GUI::drawObjectSelect(){
	int row = 0;
	int col = -1; // starts by being incrememted
	int numPerRow = 6; // it's actually 1 less than this
	int topYofGUI = AXWindow::getHeight()-(world->guiTileSize*world->tilesize);
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
		AXGraphics::drawTexture(world->objects[i]->texture, 20+(world->tilesize*col), topYofGUI+(world->tilesize*row)+20, world->tilesize, world->tilesize); 
	}
}

bool GUI::isMouseOverGUI(){
	if(AXInput::mouseY > world->maxOnScreenY*world->tilesize){
		return true;
	}
	return false;
}
void GUI::updateResources(){
	if(world->resource_pool.moneyIncome < 0){
		moneyText.reset(fontBig->bakeTexture("$"+std::to_string(world->resource_pool.money)+"("+std::to_string(world->resource_pool.moneyIncome)+")", redColour));
	}else{
		moneyText.reset(fontBig->bakeTexture("$"+std::to_string(world->resource_pool.money)+"(+"+std::to_string(world->resource_pool.moneyIncome)+")", blackColour));
	}
	if(world->resource_pool.foodIncome < 0){
		foodText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.food)+"("+std::to_string(world->resource_pool.foodIncome)+")", redColour));
	}else{
		foodText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.food)+"(+"+std::to_string(world->resource_pool.foodIncome)+")", blackColour));
	}
	if(world->resource_pool.woodIncome < 0){
		woodText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.wood)+"("+std::to_string(world->resource_pool.woodIncome)+")", redColour));
	}else{
		woodText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.wood)+"(+"+std::to_string(world->resource_pool.woodIncome)+")", blackColour));
	}
	if(world->resource_pool.stoneIncome < 0){
		stoneText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.stone)+"("+std::to_string(world->resource_pool.stoneIncome)+")", redColour));
	}else{
		stoneText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.stone)+"(+"+std::to_string(world->resource_pool.stoneIncome)+")", blackColour));
	}
	//if there's not enough food, show that people die
	if(world->resource_pool.food < 0){
		popText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.population)+"(-2)", redColour));
	}else{
		popText.reset(fontBig->bakeTexture(std::to_string(world->resource_pool.population), blackColour));
	}
}


void GUI::bakeObjectInfoStrings(std::shared_ptr<Object> obj, bool placing){
	if(!obj){
		AXLog::log("Bake Object Info", "You're trying to bake info strings when there's no object selected.", AX_LOG_ERROR);
		return;
	}
	std::shared_ptr<Object> selected = obj; // get a temp object
	//if you're placing, give an instruction, otherwise give a statement
	if(placing){
		instructionText.reset(fontBig->bakeTexture("Click to place a "+selected->name+"!", blackColour));
	}else{
		instructionText.reset(fontBig->bakeTexture("A "+selected->name+"!", blackColour));
	}
	//bake the detail1 string, it will say the type it requires or how to delete
	if(placing){
		detailText1.reset(fontSmall->bakeTexture("Tile Type: "+types[selected->requiredType], blackColour));
	}else{
		//YOU CANT DELETE THE TOWN HALL
		if(obj->id == 0){
			detailText1.reset(fontSmall->bakeTexture("This building gives you "+std::to_string(world->allowedHomeDistance)+" tiles to build on.", blackColour));
		}else{
			detailText1.reset(fontSmall->bakeTexture("Currently worth $"+std::to_string((int)selected->cost.money/2), blackColour));
		}
	}
	//create a temporary string to hold the details
	std::string detailText2String = "Costs";
	if(selected->cost.money != 0){
		detailText2String.append(" | $"+std::to_string(selected->cost.money));
	}
	if(selected->cost.stone != 0){
		detailText2String.append(" | Stone: "+std::to_string(selected->cost.stone));
	}
	if(selected->cost.wood != 0){
		detailText2String.append(" | Wood: "+std::to_string(selected->cost.wood));
	}
	detailText2.reset(fontSmall->bakeTexture(detailText2String, blackColour));
	//a string for the production
	std::string detailText3String = "Per Tick";
	if(selected->income.money != 0){
		detailText3String.append(" | $"+std::to_string(selected->income.money));
	}
	if(selected->income.food != 0){
		detailText3String.append(" | Food: "+std::to_string(selected->income.food));
	}
	if(selected->income.population != 0){
		detailText3String.append(" | Population: "+std::to_string(selected->income.population));
	}
	if(selected->income.wood != 0){
		detailText3String.append(" | Wood: "+std::to_string(selected->income.wood));
	}
	if(selected->income.stone != 0){
		detailText3String.append(" | Stone: "+std::to_string(selected->income.stone));
	}
	detailText3.reset(fontSmall->bakeTexture(detailText3String, blackColour));
}

void GUI::bakeTownText(){
	townNameText.reset(fontBig->bakeTexture(townsizes[world->townSize], blackColour));
}

void GUI::bakeSpeedText(){
	worldSpeedText.reset(fontSmall->bakeTexture("Speed: "+name_from_speed_enum(world->current_tick_speed), blackColour));
}