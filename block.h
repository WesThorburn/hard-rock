#ifndef BLOCK_H
#define BLOCK_H

#include "entity.h"

class Block{
public:
	int id = -1, typeCode = -1, x = 0, y = 0, width = 50, height = 50;
	bool active = 0, beingHit = 0;

	Block(int id, int typeCode, int x, int y);

	void draw();
};

#endif