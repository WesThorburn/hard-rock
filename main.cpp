#include <emscripten.h>
#include <iostream>
#include "blockType.h"
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

	Player* mainPlayer = &players.at(selfId);
	mainPlayer->update();
	mainPlayer->draw();
	mainPlayer->resetOneTickVariables();

	for(Block block : blocks){
		block.draw();
	}

	for(Block block : blocks){
		Block* resettingBlock = &blocks.at(block.id);
		resettingBlock->resetOneTickVariables();
	}

	drawHud();
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

	//Create main player
	Player player = Player(0, 150, -100, 0, 0, 0);
	player.setFuel(100);
	players.push_back(player);
	selfId = 0;
	camera.trackingId = selfId;

	//Generate map
	int numBlocks = 0;
	for(int row = 0; row < BLOCK_ROWS; row++){
		for(int col = 0; col < BLOCK_COLS; col++){
			BlockType type = ROCK;
			int randType = rand() % 35;
			if(randType == 1){
				type = COPPER;
			}
			else if(randType == 2){
				type = IRON;
			}
			if(col > 20 && col < 40){
				if(randType == 3){
					type = COAL;
				}
			}

			Block newBlock = Block(numBlocks, type, col * 50, row * 50);
			blocks.push_back(newBlock);
			numBlocks++;
		}
	}

	setBrowserSize(0);
	emscripten_set_main_loop(gameLoop, 0, 1);
}