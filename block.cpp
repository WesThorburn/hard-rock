#include <iostream>
#include "block.h"
#include "screen.h"
#include "drawing.h"

Block::Block(int id, BlockType typeCode, double x, double y){
	this->id = id;
	this->typeCode = typeCode;
	this->x = x;
	this->y = y;
	this->active = 1;

	if(this->typeCode == COPPER){
		this->hp = 200;
	}
	else if(this->typeCode == IRON){
		this->hp = 300;
	}
	else if(this->typeCode == COAL){
		this->hp = 400;
	}
	else if(this->typeCode == SILVER){
		this->hp = 500;
	}
	else if(this->typeCode == GOLD){
		this->hp = 600;
	}
	else if(this->typeCode == PLATINUM){
		this->hp = 700;
	}
	else{
		this->hp = 50 + (this->y / 3); //Hp varies by depth
	}
	this->maxHp = this->hp;
}

void Block::draw(){
	if(!this->active){
		return;
	}

	Location rawPosition = {this->x, this->y};
	Location relPos = camera.getRelativePosition(rawPosition);

	double opacity = 1.0;
	if(this->hp < this->maxHp){
		opacity = static_cast<double>(this->hp) / static_cast<double>(this->maxHp) + 0.4;
	}
	setGlobalAlpha(0, opacity);
	setLineWidth(0, 2);
	setStrokeStyle(0, 74, 74, 74);
	setFillStyle(0, 136, 136, 136);
	if(this->typeCode == COPPER){
		setFillStyle(0, 228, 144, 101);
	}
	else if(this->typeCode == IRON){
		setFillStyle(0, 171, 171, 171);
	}
	else if(this->typeCode == COAL){
		setFillStyle(0, 92, 94, 95);
	}
	else if(this->typeCode == SILVER){
		setFillStyle(0, 218, 243, 226);
	}
	else if(this->typeCode == GOLD){
		setFillStyle(0, 160, 149, 64);
	}
	else if(this->typeCode == PLATINUM){
		setFillStyle(0, 207, 221, 243);
	}
	else if(this->typeCode == SAPPHIRE){
		setFillStyle(0, 72, 113, 142);
	}
	else if(this->typeCode == EMERALD){
		setFillStyle(0, 43, 115, 75);
	}
	else if(this->typeCode == RUBY){
		setFillStyle(0, 125, 44, 44);
	}
	else if(this->typeCode == DIAMOND){
		setGlobalAlpha(0, 0.7);
		setFillStyle(0, 255, 255, 255);
	}

	beginPath(0);
	moveTo(0, relPos.x, relPos.y);
	lineTo(0, relPos.x + this->width - 1, relPos.y);
	lineTo(0, relPos.x + this->width - 1, relPos.y + this->height - 1);
	lineTo(0, relPos.x, relPos.y + this->height - 1);
	lineTo(0, relPos.x, relPos.y);
	stroke(0);
	fill(0);
	setGlobalAlpha(0, 1);
}

bool Block::collidesWith(double objectX, double objectY, int objectRadius){
	if(objectX + objectRadius < this->x || objectX - objectRadius > this->x + this->width){
		return false;
	}
	if(objectY + objectRadius < this->y || objectY - objectRadius > this->y + this->height){
		return false;
	}
	return true;
}

void Block::resetOneTickVariables(){
	
}