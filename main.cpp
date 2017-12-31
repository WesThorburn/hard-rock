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
	player.setFuel(5);
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
			if(row < 20){
				if(randType == 3){
					type = COPPER;
				}
				else if(randType == 4){
					type = IRON;
				}
			}
			else if(row > 20 && row <= 40){
				if(randType == 3){
					type = COAL;
				}
				else if(randType == 4){
					type = IRON;
				}
			}
			else if(row > 40 && row <= 60){
				if(randType == 3){
					type = COAL;
				}
				else if(randType == 4){
					type = SILVER;
				}
			}
			else if(row > 60 && row <= 80){
				if(randType == 3){
					type = GOLD;
				}
				else if(randType == 4){
					type = SILVER;
				}
			}
			else if(row > 80 && row <= 100){
				if(randType == 3){
					type = GOLD;
				}
				else if(randType == 4){
					type = PLATINUM;
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