#include <emscripten.h>
#include <iostream>
#include "screen.h"
#include "drawing.h"
#include "variables.h"
#include "controls.h"

void gameLoop(){
	clearCanvas(1);
	clearCanvas(0);

	camera.update();
	drawMap();

	emitCursorData();

	for(Block block : blocks){
		block.draw();
	}

	Player* mainPlayer = &players.at(selfId);
	mainPlayer->update();
	mainPlayer->draw();
}

void setGlobals(){
	EM_ASM(
		contexts = [];
		renderArea = {};
	);
}

int main(){
	srand (time(NULL)); //Seed randomizer
	setGlobals();
	buildCanvases();
	setupControls();

	//Default Camera Values
	camera.reset();
	camera.x = 1200;
	camera.y = 1200;

	//Create main player
	Player player = Player(0, 1900, 1550, 0, 0, 0);
	players.push_back(player);
	selfId = 0;
	camera.trackingId = selfId;

	//Create test boxes
	Block block1 = Block(0, 1, 2000, 1600);
	blocks.push_back(block1);
	Block block2 = Block(1, 1, 2000, 1650);
	blocks.push_back(block2);
	Block block3 = Block(2, 1, 2100, 1600);
	blocks.push_back(block3);
	Block block4 = Block(3, 1, 2000, 1550);
	blocks.push_back(block4);
	Block block5 = Block(4, 1, 2100, 1550);
	blocks.push_back(block5);

	setBrowserSize(0);
	emscripten_set_main_loop(gameLoop, 0, 1);
}