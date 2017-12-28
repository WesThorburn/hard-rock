#include <iostream>
#include <math.h>
#include <algorithm>
#include "player.h"
#include "drawing.h"
#include "screen.h"
#include "variables.h"

Player::Player(int id, double x, double y, int spdX, int spdY, int angle): Entity(id, x, y, spdX, spdY, angle){
	this->bank = 1200;
	updatePosition();
}

void Player::update(){
	if(this->fuel > 0){
		this->updateSpeed();
		this->updatePosition();
		this->handleMining();
		this->interactWithUtilities();
	}
	this->updateFuel();
}

void Player::updateSpeed(){
	this->spdY += 0.098; //Acceleration due to gravity
	double terminalVelocity = this->maxSpdY * 3;
	double xAccelerationFactor = this->maxSpdX/3;
	double yAccelerationFactor = static_cast<double>(this->maxSpdY)/15;
	if(this->pressingLeft){
		this->spdX -= xAccelerationFactor;
		direction = 2;
	}
	else if(this->pressingRight){
		this->spdX += xAccelerationFactor;
		direction = 0;
	}
	if(this->pressingUp){
		this->spdY -= yAccelerationFactor;
		direction = 3;
	}
	else if(this->pressingDown){
		direction = 1;
	}

	if(this->spdX < -this->maxSpdX){
		this->spdX = -this->maxSpdX;
	}
	else if(this->spdX > this->maxSpdX){
		this->spdX = this->maxSpdX;
	}
	if(this->spdY < -this->maxSpdY * 3){ //maxSpdY gets a 3x boost
		this->spdY = -this->maxSpdY * 3;
	}
	else if(this->spdY > terminalVelocity){
		this->spdY = terminalVelocity;
	}

	this->angle = direction * 90; //Angle locks to directions 0, 1, 2

	//x-axis friction
	this->spdX *= 0.92;

	//Collision detection
	std::vector<int> surroundingBlockIds = {
		(this->row * BLOCK_COLS) + this->col,
		((this->row - 1) * BLOCK_COLS) + (this->col - 1),
		((this->row - 1) * BLOCK_COLS) + this->col,
		((this->row - 1) * BLOCK_COLS) + (this->col + 1),
		(this->row * BLOCK_COLS) + (this->col - 1),
		(this->row * BLOCK_COLS) + (this->col + 1),
		((this->row + 1) * BLOCK_COLS) + (this->col - 1),
		((this->row + 1) * BLOCK_COLS) + this->col,
		((this->row + 1) * BLOCK_COLS) + (this->col + 1)
	};

	double testX = this->x + this->spdX;
	double testY = this->y + this->spdY;

	//Y-Collision Check
	for(auto &blockId : surroundingBlockIds){
		if(blockId < 0 || blockId > blocks.size()){
			continue;
		}
		Block* block = &blocks.at(blockId);
		if(!block->active){
			continue;
		}
		if(block->collidesWith(testX, this->y, this->radius)){ //X Axis check
			this->spdX = 0;
		}
		if(block->collidesWith(this->x, testY, this->radius)){ //Y Axis check
			this->spdY = 0;
		}
	}

	//Stop speeds from reaching tiny numbers
	if(this->spdX > -0.05 && this->spdX < 0.05){
		this->spdX = 0;
	}
	if(this->spdY > -0.05 && this->spdY < 0.05){
		this->spdY = 0;
	}
}

void Player::updatePosition(){
	this->x += this->spdX;
	this->y += this->spdY;

	this->row = this->y / 50;
	this->col = this->x / 50;
}

void Player::handleMining(){
	if((this->y < 0 && direction != 1) || direction == 3){ //Ignore horizontal mining above surface, ignore upward mining
		return;
	}

	if((this->spdY != 0) || (this->spdX != 0 && (this->pressingLeft || this->pressingRight)) || (this->spdY != 0 && this->pressingDown)){
		return;
	}
	else if(this->pressingDown || this->pressingLeft || this->pressingRight){
		int blockId = 0;
		switch(direction){
			case 0:
				blockId = (this->row * BLOCK_COLS) + (this->col + 1);
				break;
			case 1:
				blockId = ((this->row + 1) * BLOCK_COLS) + this->col;
				break;
			case 2:
				blockId = (this->row * BLOCK_COLS) + (this->col - 1);
				break;
		}

		//Special case for surface row
		if(this->y < 0 && direction == 1){
			blockId = ((this->row) * BLOCK_COLS) + this->col;
		}

		if(blockId >= 0 && blockId < blocks.size()){
			this->mining = 1;
			Block* blockToBeMined = &blocks.at(blockId);
			blockToBeMined->beingMined = 1;
			blockToBeMined->hp -= this->drillStrength;
			if(blockToBeMined->hp <= 0){
				blockToBeMined->hp = 0;
				blockToBeMined->active = 0;
				this->cargo.increment(blockToBeMined->typeCode);
			}
		}
		
		if(this->mining){
			int shakeDirection = rand() % 2;
			if(shakeDirection == 0){
				this->angle++;
			}
			else{
				this->angle--;
			}
		}
	}
}

void Player::updateFuel(){
	this->fuel -= 0.005;
	if(this->mining){
		this->fuel -= 0.025;
	}

	if(this->fuel < 0){
		this->fuel = 0;
	}
}

void Player::interactWithUtilities(){
	//Fuel Station
	if(this->x > 1000 && this->x < 1100 && this->y > -100 && this->y < 0){
		fillTank();
	}

	//Market
	if(this->x > 1400 && this->x < 1500 && this->y > -100 && this->y < 0){
		this->bank += cargo.getTotalValue();
		this->cargo.reset();
	}
}

void Player::draw(){
	if(this->radius > 0){
		Location rawPosition = {this->x, this->y};
		Location relPos = camera.getRelativePosition(rawPosition);

		setGlobalAlpha(0, 1);
		setLineWidth(0, 2);
		setStrokeStyle(0, 74, 74, 74);
		setFillStyle(0, 74, 74, 74);

		//Tracks
		double trackFrontLeftX = cos((this->angle + 295) * (M_PI/180)) * this->radius + relPos.x;
		double trackFrontLeftY = sin((this->angle + 295) * (M_PI/180)) * this->radius + relPos.y;
		double trackFrontRightX = cos((this->angle + 65) * (M_PI/180)) * this->radius + relPos.x;
		double trackFrontRightY = sin((this->angle + 65) * (M_PI/180)) * this->radius + relPos.y;
		double trackBackRightX = cos((this->angle + 115) * (M_PI/180)) * this->radius + relPos.x;
		double trackBackRightY = sin((this->angle + 115) * (M_PI/180)) * this->radius + relPos.y;
		double trackBackLeftX = cos((this->angle + 245) * (M_PI/180)) * this->radius + relPos.x;
		double trackBackLeftY = sin((this->angle + 245) * (M_PI/180)) * this->radius + relPos.y;

		beginPath(0);
		moveTo(0, trackFrontLeftX, trackFrontLeftY);
		lineTo(0, trackFrontRightX, trackFrontRightY);
		lineTo(0, trackBackRightX, trackBackRightY);
		lineTo(0, trackBackLeftX, trackBackLeftY);
		lineTo(0, trackFrontLeftX, trackFrontLeftY);
		fill(0);
		stroke(0);

		//Drill mount
		if(this->direction != 3){
			double drillMountX = cos((this->angle + 0) * (M_PI/180)) * (this->radius * 1.4) + relPos.x;
			double drillMountY = sin((this->angle + 0) * (M_PI/180)) * (this->radius * 1.4) + relPos.y;
			setLineWidth(0, 5);
			drawLine(0, relPos.x, relPos.y, drillMountX, drillMountY);
		}

		setFillStyle(0, 136, 136, 136);
		setLineWidth(0, 2);

		//Rear
		double rearStartLeftX = cos((this->angle + 215) * (M_PI/180)) * (this->radius * 0.8) + relPos.x;
		double rearStartLeftY = sin((this->angle + 215) * (M_PI/180)) * (this->radius * 0.8) + relPos.y;
		double rearEndLeftX = cos((this->angle + 200) * (M_PI/180)) * this->radius + relPos.x;
		double rearEndLeftY = sin((this->angle + 200) * (M_PI/180)) * this->radius + relPos.y;
		double rearStartRightX = cos((this->angle + 145) * (M_PI/180)) * (this->radius * 0.8) + relPos.x;
		double rearStartRightY = sin((this->angle + 145) * (M_PI/180)) * (this->radius * 0.8) + relPos.y;
		double rearEndRightX = cos((this->angle + 160) * (M_PI/180)) * this->radius + relPos.x;
		double rearEndRightY = sin((this->angle + 160) * (M_PI/180)) * this->radius + relPos.y;

		beginPath(0);
		moveTo(0, rearStartLeftX, rearStartLeftY);
		lineTo(0, rearEndLeftX, rearEndLeftY);
		lineTo(0, rearEndRightX, rearEndRightY);
		lineTo(0, rearStartRightX, rearStartRightY);
		lineTo(0, rearStartLeftX, rearStartLeftY);
		fill(0);
		stroke(0);

		//Body
		double backLeftCornerX = cos((this->angle + 225) * (M_PI/180)) * this->radius + relPos.x;
		double backLeftCornerY = sin((this->angle + 225) * (M_PI/180)) * this->radius + relPos.y;

		double leftOuterCurveX = cos((this->angle + 310) * (M_PI/180)) * (this->radius * 0.9) + relPos.x;
		double leftOuterCurveY = sin((this->angle + 310) * (M_PI/180)) * (this->radius * 0.9) + relPos.y;

		double frontPointLeftX = cos((this->angle + 340) * (M_PI/180)) * this->radius + relPos.x;
		double frontPointLeftY = sin((this->angle + 340) * (M_PI/180)) * this->radius + relPos.y;
		double frontPointRightX = cos((this->angle + 20) * (M_PI/180)) * this->radius + relPos.x;
		double frontPointRightY = sin((this->angle + 20) * (M_PI/180)) * this->radius + relPos.y;

		double rightOuterCurveX = cos((this->angle + 50) * (M_PI/180)) * (this->radius * 0.9) + relPos.x;
		double rightOuterCurveY = sin((this->angle + 50) * (M_PI/180)) * (this->radius * 0.9) + relPos.y;

		double backRightCornerX = cos((this->angle + 135) * (M_PI/180)) * this->radius + relPos.x;
		double backRightCornerY =  sin((this->angle + 135) * (M_PI/180)) * this->radius + relPos.y;

		setLineCap(0, 1);
		setLineJoin(0, 1);
		beginPath(0);
		moveTo(0, backLeftCornerX, backLeftCornerY);
		lineTo(0, backRightCornerX, backRightCornerY);
		lineTo(0, rightOuterCurveX, rightOuterCurveY);
		lineTo(0, frontPointRightX, frontPointRightY);
		lineTo(0, frontPointLeftX, frontPointLeftY);
		lineTo(0, leftOuterCurveX, leftOuterCurveY);
		lineTo(0, backLeftCornerX, backLeftCornerY);
		fill(0);
		stroke(0);

		//Drill
		if(this->direction != 3){
			double drillTipX = cos((this->angle + 0) * (M_PI/180)) * (this->radius * 1.7) + relPos.x;
			double drillTipY = sin((this->angle + 0) * (M_PI/180)) * (this->radius * 1.7) + relPos.y;
			double drillStartLeftX = cos((this->angle + 330) * (M_PI/180)) * (this->radius * 1.2) + relPos.x;
			double drillStartLeftY = sin((this->angle + 330) * (M_PI/180)) * (this->radius * 1.2) + relPos.y;
			double drillStartRightX = cos((this->angle + 30) * (M_PI/180)) * (this->radius * 1.2) + relPos.x;
			double drillStartRightY = sin((this->angle + 30) * (M_PI/180)) * (this->radius * 1.2) + relPos.y;

			beginPath(0);
			moveTo(0, drillStartLeftX, drillStartLeftY);
			lineTo(0, drillStartRightX, drillStartRightY);
			lineTo(0, drillTipX, drillTipY);
			lineTo(0, drillStartLeftX, drillStartLeftY);
			fill(0);
			stroke(0);
		}

		//Outline
		/*beginPath(0);
		setLineWidth(0, 1);
		setStrokeStyle(0, 255, 0, 0);
		arc(0, relPos.x, relPos.y, this->radius);
		stroke(0);*/

		//Hp Bar
		if(this->hp > 0 && this->hp < this->maxHp){
			int hpBarBackingWidth = this->radius * 2.5;
			int hpBarHeight = round(hpBarBackingWidth * 0.12);
			int hpBarCornerRadius = hpBarHeight * 0.2;
			int hpBarXOffset = relPos.x - hpBarBackingWidth * 0.5;
			int hpBarYOffset = relPos.y + this->radius * 1.6;
			setLineWidth(0, 1);

			//Backing
			setFillStyle(0, 74, 74, 74);
			drawCurvedRectangle(0, hpBarXOffset, hpBarYOffset, hpBarBackingWidth, hpBarHeight, hpBarCornerRadius);
			fill(0);

			//Health content
			setFillStyle(0, 66, 244, 134);
			double hpPercentage = (float)this->hp / (float)this->maxHp;
			int remainingHPWidth = hpBarBackingWidth * hpPercentage;

			drawCurvedRectangle(0, hpBarXOffset + 1, hpBarYOffset + 1, remainingHPWidth - 2, hpBarHeight - 2, (hpBarHeight - 2) * 0.2);
			fill(0);
		}
	}
}

void Player::setFuel(double liters){
	if(liters > 0){
		this->fuel = liters;
	}
	if(this->fuel > this->maxFuel){
		this->fuel = this->maxFuel;
	}
}

void Player::fillTank(){
	if(this->bank < 10 || this->fuel >= this->maxFuel - 1){
		return;
	}
	setFuel(this->fuel + 1);
	this->bank = this->bank - 10;
}

double Player::getFuel(){
	return this->fuel;
}

void Player::resetOneTickVariables(){
	this->mining = 0;
}