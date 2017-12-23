#ifndef ENTITY_H
#define ENTITY_H

class Entity{
public:
	int id = 0, spdX = 0, spdY = 0, angle = 0;
	double x = 0, y = 0;
	bool activated = 0;

	Entity(int id, double x, double y, int spdX, int spdY, int angle);
};

#endif