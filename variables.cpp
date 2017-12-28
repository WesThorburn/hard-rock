#include "variables.h"

const int MAX_MAP_WIDTH = 7000;
const int MAX_MAP_HEIGHT = 7000;
const int BLOCK_ROWS = 500;
const int BLOCK_COLS = 200;

bool spectating = 0;
bool debug = 1;
bool windowFocus = 1; //When false, window is blurred
int resizeWaitTimer = 0;
int selfId = 0;

bool clickingHud = 0;

std::vector<Player> players;
std::vector<Block> blocks;

void resetOneTickVariables(){
	
}