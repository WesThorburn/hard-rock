#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "entity.h"

class Player: public Entity{
public:
	int teamCode = -1, radius = 18, hp = 0, maxHp = 0, selectedWeapon = -1, score = 0, kills = 0, maxSpd = 3, drillStrength = 3;;
	double absSpd = 0, spdX = 0, spdY = 0;
	bool pressingForward = 0, pressingBackward = 0, pressingMine = 0, mining = 0;
	int cameraWidth = 0, cameraHeight = 0;

	Player(int id, double x, double y, int spdX, int spdY, int angle);

	void update();
	void updateSpeed();
	void updatePosition();
	void handleMining();
	void draw();
	void resetOneTickVariables();
};

#endif