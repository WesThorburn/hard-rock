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
}

void Block::draw(){
	if(!this->active){
		return;
	}

	Location rawPosition = {this->x, this->y};
	Location relPos = camera.getRelativePosition(rawPosition);

	double opacity = 1.0;
	if(this->hp < this->maxHp){
		opacity = static_cast<double>(this->hp) / static_cast<double>(this->maxHp);
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

	beginPath(0);
	moveTo(0, relPos.x, relPos.y);
	lineTo(0, relPos.x + this->width - 1, relPos.y);
	lineTo(0, relPos.x + this->width - 1, relPos.y + this->height - 1);
	lineTo(0, relPos.x, relPos.y + this->height - 1);
	lineTo(0, relPos.x, relPos.y);
	stroke(0);
	fill(0);
}

void Block::resetOneTickVariables(){
	
}