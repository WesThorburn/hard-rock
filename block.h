#ifndef BLOCK_H
#define BLOCK_H

#include "entity.h"

class Block{
public:
	int id = -1, typeCode = -1, width = 50, height = 50, hp = 100;
	bool active = 0, beingMined = 0;
	double x = 0, y = 0;

	Block(int id, int typeCode, double x, double y);

	void draw();
	void resetOneTickVariables();
};

#endif