#include <iostream>
#include <math.h>
#include <algorithm>
#include "player.h"
#include "drawing.h"
#include "screen.h"
#include "variables.h"

Player::Player(int id, double x, double y, int spdX, int spdY, int angle): Entity(id, x, y, spdX, spdY, angle){
}

void Player::update(){
	if(this->fuel > 0){
		this->updateSpeed();
		this->updatePosition();
		this->handleMining();
	}
	this->updateFuel();
}

void Player::updateSpeed(){
	double accelerationFactor = this->maxSpd/3;

	if(this->pressingForward){
		this->absSpd -= accelerationFactor;
	}
	if(this->pressingBackward){
		this->absSpd += accelerationFactor;
	}

	//Limit to maxSpd
	if(this->absSpd > this->maxSpd){
		this->absSpd = this->maxSpd;
	}
	else if(this->absSpd < -this->maxSpd){
		this->absSpd = -this->maxSpd;
	}

	this->spdX = cos((this->angle - 180) * (M_PI/180)) * this->absSpd;
	this->spdY = sin((this->angle - 180) * (M_PI/180)) * this->absSpd;

	//friction
	this->spdX *= 0.88;
	this->spdY *= 0.88;
	this->absSpd *= 0.88;

	if(this->spdX < 0.5 && this->spdX > -0.5){
		this->spdX = 0;
	}
	if(this->spdY < 0.5 && this->spdY > -0.5){
		this->spdY = 0;
	}
	if(this->absSpd < 0.5 && this->absSpd > -0.5){
		this->absSpd = 0;
	}

	//Check collisions
	double testXPosition = this->x + this->spdX;
	double testYPosition = this->y + this->spdY;
	double reboundSpeed = 0.3;
	for(Block checkingBlock : blocks){
		Block* block = &blocks.at(checkingBlock.id);
		if(!block->active){
			continue;
		}
		bool blockCollision = 0;
		bool xAxisCollision = 0;
		bool yAxisCollision = 0;
		double deltaX = 0.0;
		double deltaY = 0.0;

		deltaX = testXPosition - std::max(block->x, std::min(testXPosition, block->x + block->width));
		deltaY = this->y - std::max(block->y, std::min(this->y, block->y + block->height));
		if((pow(deltaX, 2) + pow(deltaY, 2)) < (pow(this->radius, 2))){
			blockCollision = 1;
			xAxisCollision = 1;
		}

		deltaX = this->x - std::max(block->x, std::min(this->x, block->x + block->width));
		deltaY = testYPosition - std::max(block->y, std::min(testYPosition, block->y + block->height));
		if((pow(deltaX, 2) + pow(deltaY, 2)) < (pow(this->radius, 2))){
			blockCollision = 1;
			yAxisCollision = 1;
		}

		if(xAxisCollision && yAxisCollision){
			if(this->x <= block->x && this->y <= block->y){
				if(this->spdX <= this->spdY){
					this->spdX = -reboundSpeed;
					this->spdY = 0;
				}
				else{
					this->spdX = 0;
					this->spdY = -reboundSpeed;
				}
			}
			else if(this->x > block->x + block->width && this->y <= block->y){
				if(this->spdY > 0 && this->spdY >= this->spdX){
					this->spdX = reboundSpeed;
					this->spdY = 0;
				}
				else{
					this->spdX = 0;
					this->spdY = -reboundSpeed;
				}
			}
			else if(this->x <= block->x && this->y > block->y + block->height){
				if(this->spdX > 0 && this->spdX >= this->spdY){
					this->spdX = 0;
					this->spdY = reboundSpeed;
				}
				else{
					this->spdX = -reboundSpeed;
					this->spdY = 0;
				}
			}
			else if(this->x > block->x + block->width && this->y > block->y + block->height){
				if(this->spdX >= this->spdY){
					this->spdX = reboundSpeed;
					this->spdY = 0;
				}
				else{
					this->spdX = 0;
					this->spdY = reboundSpeed;
				}
			}
		}
		if(xAxisCollision && !yAxisCollision){
			this->spdX = 0;
		}
		if(yAxisCollision && !xAxisCollision){
			this->spdY = 0;
		}
	}
}

void Player::updatePosition(){
	this->x += this->spdX;
	this->y += this->spdY;
}

void Player::handleMining(){
	if(this->pressingMine){
		Location drillTip = {
			this->x + cos((this->angle) * (M_PI/180)) * (this->radius * 1.7), 
			this->y + sin((this->angle) * (M_PI/180)) * (this->radius * 1.7)
		};
		int row = drillTip.y/50;
		int col = drillTip.x/50;
		int id = (row * BLOCK_COLS) + col;
		if(id < blocks.size()){
			this->mining = 1;
			Block* blockToBeMined = &blocks.at(id);
			blockToBeMined->beingMined = 1;
			blockToBeMined->hp -= this->drillStrength;
			if(blockToBeMined->hp <= 0){
				blockToBeMined->hp = 0;
				blockToBeMined->active = 0;
			}
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

void Player::updateFuel(){
	this->fuel -= 0.05;
	if(this->mining){
		this->fuel -= 0.025;
	}

	if(this->fuel < 0){
		this->fuel = 0;
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
		double drillMountX = cos((this->angle + 0) * (M_PI/180)) * (this->radius * 1.4) + relPos.x;
		double drillMountY = sin((this->angle + 0) * (M_PI/180)) * (this->radius * 1.4) + relPos.y;
		setLineWidth(0, 5);
		drawLine(0, relPos.x, relPos.y, drillMountX, drillMountY);

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
}

std::string Player::getFuel(){
	return std::to_string(this->fuel) + "L";
}

void Player::resetOneTickVariables(){
	this->mining = 0;
}