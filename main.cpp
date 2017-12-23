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
	mainPlayer->resetOneTickVariables();

	for(Block block : blocks){
		Block* resettingBlock = &blocks.at(block.id);
		resettingBlock->resetOneTickVariables();
	}
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
	Player player = Player(0, 600, 300, 0, 0, 0);
	players.push_back(player);
	selfId = 0;
	camera.trackingId = selfId;

	//Create test boxes
	int numBlocks = 0;
	for(int row = 0; row < 10; row++){
		for(int col = 0; col < 10; col++){
			Block newBlock = Block(numBlocks, 1, row * 50, col * 50);
			blocks.push_back(newBlock);
			numBlocks++;
		}
	}

	setBrowserSize(0);
	emscripten_set_main_loop(gameLoop, 0, 1);
}