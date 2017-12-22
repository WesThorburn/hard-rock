#include "entity.h"

Entity::Entity(int id, int x, int y, int spdX, int spdY, int angle){
	this->id = id;
	this->x = x;
	this->y = y;
	this->spdX = spdX;
	this->spdY = spdY;
	this->angle = angle;
}