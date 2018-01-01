#ifndef BLOCK_H
#define BLOCK_H

#include "entity.h"

class Block{
public:
	enum Type{
		ROCK,
		COPPER,
		IRON,
		COAL,
		SILVER,
		GOLD,
		PLATINUM,
		SAPPHIRE,
		EMERALD,
		RUBY,
		DIAMOND,
	};
	int id = -1, width = 50, height = 50, hp = 100, maxHp = 100;
	Type typeCode = ROCK;
	bool active = 0, beingMined = 0;
	double x = 0, y = 0;

	Block(int id, Type type, double x, double y);

	void draw();
	bool collidesWith(double x, double y, int radius);
	void resetOneTickVariables();
};

#endif