#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "entity.h"

class Player: public Entity{
public:
	int teamCode = -1, radius = 24, hp = 0, maxHp = 0, selectedWeapon = -1, score = 0, kills = 0, maxSpd = 3;
	double absSpd = 0, spdX = 0, spdY = 0;
	bool pressingForward = 0, pressingBackward = 0;
	int cameraWidth = 0, cameraHeight = 0;

	Player(int id, int x, int y, int spdX, int spdY, int angle);

	void update();
	void updateSpeed();
	void updatePosition();
	void draw();
};

#endif