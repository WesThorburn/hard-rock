#include "camera.h"
#include "player.h"
#include "variables.h"

Camera::Camera(){};

void Camera::update(){
	if(trackingId >= 0){
		Player trackingPlayer = players.at(trackingId);
		this->x = trackingPlayer.x - this->windowSizeOffset.x;
		this->y = trackingPlayer.y - this->windowSizeOffset.y;
	}
}

void Camera::reset(){
	this->width = 1366;
	this->height = 768;
	this->spdX = 0;
	this->spdY = 0;
}

void Camera::trackPlayer(int playerId){
	this->trackingId = playerId;
}

Location Camera::getRelativePosition(Location entityLocation){
	Location relativePosition;
	relativePosition.x = entityLocation.x - this->x;
	relativePosition.y = entityLocation.y - this->y;
	return relativePosition;
}
Location Camera::getPosition(){
	Location position;
	position.x = this->x;
	position.y = this->y;
	return position;
}