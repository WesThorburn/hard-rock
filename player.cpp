#include <iostream>
#include <math.h>
#include <algorithm>
#include "player.h"
#include "drawing.h"
#include "screen.h"
#include "variables.h"

Player::Player(int id, double x, double y, int spdX, int spdY, int angle): Entity(id, x, y, spdX, spdY, angle){
	updatePosition();
}

void Player::update(){
	this->updateSpeed();
	this->updatePosition();
	this->handleMining();
	this->interactWithUtilities();
	this->updateFuel();
}

void Player::updateSpeed(){
	if(this->fuel <= 0){
		disconnectUserInputs();
	}

	this->spdY += 0.098; //Acceleration due to gravity
	double terminalVelocity = this->maxSpdY * 3;
	double xAccelerationFactor = this->maxSpdX/3;
	double yAccelerationFactor = static_cast<double>(this->maxSpdY)/15;
	if(this->pressingLeft){
		this->spdX -= xAccelerationFactor;
		direction = 2;
		windowDirection = 0;
	}
	else if(this->pressingRight){
		this->spdX += xAccelerationFactor;
		direction = 0;
		windowDirection = 1;
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
			if(blockToBeMined->active && blockToBeMined->hp <= 0){
				blockToBeMined->hp = 0;
				blockToBeMined->active = 0;
				this->cargo.increment(blockToBeMined->typeCode);
			}
		}
		
		if(this->mining){
			int shakeDirection = rand() % 2;
			if(shakeDirection == 0){
				this->angle += 1;
			}
			else{
				this->angle -= 1;
			}
		}
	}
}

void Player::updateFuel(){
	this->fuel -= 0.005;
	if(this->mining){
		this->fuel -= 0.055;
	}

	if(this->fuel < 0){
		this->fuel = 0;
	}
}

void Player::interactWithUtilities(){
	//Upgrade Shop
	if(this->x > 600 && this->x < 700 && this->y > -100 && this->y < 0){
		this->insideUpgradeShop = 1;
	}
	else{
		this->insideUpgradeShop = 0;
	}

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

		//Drill mount
		if(this->direction != 3){
			double drillMountX = cos((this->angle) * (M_PI/180)) * (this->radius * 1.2) + relPos.x;
			double drillMountY = sin((this->angle) * (M_PI/180)) * (this->radius * 1.2) + relPos.y;
			setLineWidth(0, 10);
			drawLine(0, relPos.x, relPos.y, drillMountX, drillMountY);
		}

		//Jet blast
		if(this->pressingUp){
			setLineWidth(0, 1);
			setGlobalAlpha(0, 0.6);
			setStrokeStyle(0, 248, 192, 132);
			setFillStyle(0, 248, 192, 132);
			beginPath(0);
			moveTo(0, (relPos.x - this->radius * 0.3), relPos.y + this->radius);
			lineTo(0, (relPos.x - this->radius * 0.3), relPos.y + this->radius * 2);
			lineTo(0, (relPos.x - this->radius * 0.15), relPos.y + this->radius * 2.7);
			lineTo(0, relPos.x, relPos.y + this->radius * 3);
			lineTo(0, (relPos.x + this->radius * 0.15), relPos.y + this->radius * 2.7);
			lineTo(0, (relPos.x + this->radius * 0.3), relPos.y + this->radius * 2);
			lineTo(0, (relPos.x + this->radius * 0.3), relPos.y + this->radius);
			lineTo(0, (relPos.x - this->radius * 0.3), relPos.y + this->radius);
			stroke(0);
			fill(0);

			setStrokeStyle(0, 201, 246, 255);
			setFillStyle(0, 201, 246, 255);
			beginPath(0);
			moveTo(0, (relPos.x - this->radius * 0.5), relPos.y + this->radius);
			lineTo(0, (relPos.x - this->radius * 0.5), relPos.y + this->radius * 1.5);
			lineTo(0, (relPos.x - this->radius * 0.3), relPos.y + this->radius * 1.35);
			lineTo(0, (relPos.x - this->radius * 0.1), relPos.y + this->radius * 1.45);
			lineTo(0, (relPos.x + this->radius * 0.1), relPos.y + this->radius * 1.45);
			lineTo(0, (relPos.x + this->radius * 0.3), relPos.y + this->radius * 1.35);
			lineTo(0, (relPos.x + this->radius * 0.5), relPos.y + this->radius * 1.5);
			lineTo(0, (relPos.x + this->radius * 0.5), relPos.y + this->radius);
			lineTo(0, (relPos.x - this->radius * 0.5), relPos.y + this->radius);
			stroke(0);
			fill(0);
		}

		//Body
		setGlobalAlpha(0, 1);
		setLineWidth(0, 4);
		setStrokeStyle(0, 74, 74, 74);
		setFillStyle(0, 136, 136, 136);
		drawCurvedRectangle(0, relPos.x - this->radius + 1, relPos.y - this->radius + 1, this->radius * 2 - 2, this->radius * 1.6 - 2, 5);
		fill(0);

		//Tracks
		drawCurvedRectangle(0, relPos.x - this->radius + 1, relPos.y + (this->radius * 0.8) - 1, this->radius * 2 - 2, this->radius * 0.2, 1);
		fill(0);

		//Window
		setFillStyle(0, 74, 74, 74);
		int windowXPosition = relPos.x - this->radius + 1;
		if(this->windowDirection == 1){
			windowXPosition = relPos.x + this->radius - (this->radius/2) - 1;
		}
		drawCurvedRectangle(0, windowXPosition, relPos.y - (this->radius * 0.7), this->radius/2, this->radius * 0.2, 1);
		fill(0);

		//Drill
		if(this->direction != 3){
			double drillTipX = cos((this->angle) * (M_PI/180)) * (this->radius * 1.8) + relPos.x;
			double drillTipY = sin((this->angle) * (M_PI/180)) * (this->radius * 1.8) + relPos.y;
			double drillStartLeftX = cos((this->angle + 330) * (M_PI/180)) * (this->radius * 1.45) + relPos.x;
			double drillStartLeftY = sin((this->angle + 330) * (M_PI/180)) * (this->radius * 1.45) + relPos.y;
			double drillStartRightX = cos((this->angle + 30) * (M_PI/180)) * (this->radius * 1.45) + relPos.x;
			double drillStartRightY = sin((this->angle + 30) * (M_PI/180)) * (this->radius * 1.45) + relPos.y;

			setFillStyle(0, 136, 136, 136);
			setLineJoin(0, 1);
			setLineCap(0, 1);
			setLineWidth(0, 2);
			beginPath(0);
			moveTo(0, drillStartLeftX, drillStartLeftY);
			lineTo(0, drillStartRightX, drillStartRightY);
			lineTo(0, drillTipX, drillTipY);
			lineTo(0, drillStartLeftX, drillStartLeftY);
			fill(0);
			stroke(0);
		}

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
	if(this->bank < 3 || this->fuel >= this->maxFuel - 1){
		return;
	}
	setFuel(this->fuel + 1);
	this->bank = this->bank - 3;
}

void Player::upgrade(int itemCode){
	int upgradeCost = getUpgradeCost(itemCode);
	switch(itemCode){
		case 0: //Fuel tank
			if(this->bank >= upgradeCost){
				this->fuelTankLevel++;
				this->maxFuel *= 1.3;
				this->bank -= upgradeCost;
			}
			break;
		case 1: //Drill
			if(this->bank >= upgradeCost){
				this->drillStrengthLevel++;
				this->drillStrength *= 1.3;
				this->bank -= upgradeCost;
			}
			break;
	}
}

int Player::getUpgradeCost(int itemCode){
	switch(itemCode){
		case 0:
			return 1000 + (this->fuelTankLevel * 1000);
		case 1:
			return 1000 + (this->drillStrengthLevel * 1000);
	}
	return 0;
}

double Player::getFuel(){
	return this->fuel;
}

void Player::disconnectUserInputs(){
	this->pressingLeft = 0;
	this->pressingRight = 0;
	this->pressingUp = 0;
	this->pressingDown = 0;
}

void Player::resetOneTickVariables(){
	this->mining = 0;
}