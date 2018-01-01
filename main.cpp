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

	for(Block &block : blocks){
		if(block.x > mainPlayer->x - 700 && block.x < mainPlayer->x + 600 && 
		block.y > mainPlayer->y - 400 && block.y < mainPlayer->y + 300){
			block.draw();
		}
	}

	for(Block &block : blocks){
		if(block.x > mainPlayer->x - 700 && block.x < mainPlayer->x + 600 && 
		block.y > mainPlayer->y - 400 && block.y < mainPlayer->y + 300){
			Block* resettingBlock = &blocks.at(block.id);
			resettingBlock->resetOneTickVariables();
		}
	}

	drawHud();
	drawUserInterfaces();
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
	Player player = Player(0, 1000, -100, 0, 0, 0);
	player.setFuel(100);
	players.push_back(player);
	selfId = 0;
	camera.trackingId = selfId;

	//Generate map
	int numBlocks = 0;
	for(int row = 0; row < BLOCK_ROWS; row++){
		for(int col = 0; col < BLOCK_COLS; col++){
			BlockType type = ROCK;
			int oreChance = rand() % 4;
			if(row > 0 && oreChance == 0){
				int rockVariety = (row/10) + 1;
				int randType = rand() % rockVariety;
				switch(randType){
					case 1:
						type = COPPER;
						break;
					case 2:
						type = IRON;
						break;
					case 3:
						type = COAL;
						break;
					case 4:
						type = SILVER;
						break;
					case 5:
						type = GOLD;
						break;
					case 6:
						type = PLATINUM;
						break;
					case 7:
						type = SAPPHIRE;
						break;
					case 8:
						type = EMERALD;
						break;
					case 9:
						type = RUBY;
						break;
					case 10:
						type = DIAMOND;
						break;
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