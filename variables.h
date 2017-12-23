#ifndef VARIABLES_H
#define VARIABLES_H

#include <vector>
#include <string>
#include "player.h"
#include "block.h"
#include "location.h"

extern const int MAX_MAP_WIDTH;
extern const int MAX_MAP_HEIGHT;
extern const int BLOCK_ROWS;
extern const int BLOCK_COLS;

extern bool spectating;
extern bool debug;
extern bool windowFocus;
extern int resizeWaitTimer;
extern int selfId;

extern bool clickingHud;

extern std::vector<Player> players;
extern std::vector<Block> blocks;

void resetOneTickVariables();

#endif