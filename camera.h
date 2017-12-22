#ifndef CAMERA_H
#define CAMERA_H

#include "location.h"

class Camera{
public:
	int x;
	int y;
	float spdX;
	float spdY;
	int width;
	int height;
	int trackingId = -1;
	Location windowSizeOffset; //x & y values of camera offset

	Camera();

	void update();
	void reset();
	void trackPlayer(int playerId);
	Location getRelativePosition(Location entityLocation);
	Location getPosition();
};

#endif