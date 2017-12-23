#include "block.h"
#include "screen.h"
#include "drawing.h"

Block::Block(int id, int typeCode, double x, double y){
	this->id = id;
	this->typeCode = typeCode;
	this->x = x;
	this->y = y;
	this->active = 1;
}

void Block::draw(){
	if(!this->active){
		return;
	}

	Location rawPosition = {this->x, this->y};
	Location relPos = camera.getRelativePosition(rawPosition);

	setGlobalAlpha(0, 1);
	setLineWidth(0, 2);
	setStrokeStyle(0, 74, 74, 74);
	setFillStyle(0, 136, 136, 136);
	if(this->beingMined){
		setFillStyle(0, 255, 0, 0);
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