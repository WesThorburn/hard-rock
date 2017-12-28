#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include "entity.h"
#include "payload.h"

class Player: public Entity{
public:
	int teamCode = -1, radius = 18, hp = 0, maxHp = 0, selectedWeapon = -1, score = 0, kills = 0;
	int bank = 1200, drillStrength = 10, maxFuel = 100;
	int row = -1, col = -1, direction = 0; //0:right, 1:down, 2:left, 3:up
	double absSpd = 0, spdX = 0, spdY = 0, maxSpdX = 3, maxSpdY = 3;
	int turretAngle = 0;
	bool pressingLeft = 0, pressingRight = 0, pressingUp = 0, pressingDown = 0, mining = 0;
	int cameraWidth = 0, cameraHeight = 0;
	Payload cargo;

	Player(int id, double x, double y, int spdX, int spdY, int angle);

	void update();
	void updateSpeed();
	void updatePosition();
	void handleMining();
	void updateFuel();
	void interactWithUtilities();
	void draw();
	void setFuel(double liters);
	double getFuel();
	void fillTank();
	void resetOneTickVariables();

private:
	double fuel = 0.0;
};

#endif