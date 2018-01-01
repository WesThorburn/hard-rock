#include <emscripten.h>
#include <iostream>
#include "drawing.h"
#include "variables.h"
#include "location.h"
#include "screen.h"
#include "controls.h"

void setFillStyle(int ctxNum, int r, int g, int b){
	EM_ASM_({
		contexts[$0].fillStyle = "rgb(" + $1 + ", " + $2 + ", " + $3 + ")";
	}, ctxNum, r, g, b);
}

void setStrokeStyle(int ctxNum, int r, int g, int b){
	EM_ASM_({
		contexts[$0].strokeStyle = "rgb(" + $1 + ", " + $2 + ", " + $3 + ")";
	}, ctxNum, r, g, b);
}

void setGlobalAlpha(int ctxNum, float globalAlpha){
	EM_ASM_({
		contexts[$0].globalAlpha = $1;
	}, ctxNum, globalAlpha);
}

void setLineWidth(int ctxNum, int lineWidth){
	EM_ASM_({
		contexts[$0].lineWidth = $1;
	}, ctxNum, lineWidth);
}

void setCursorPointer(){
	EM_ASM({
		document.getElementById("canvas").style.cursor = "pointer";
	});
}

void setCursorDefault(){
	EM_ASM({
		document.getElementById("canvas").style.cursor = "default";
	});
}

void drawRectangle(int ctxNum, int x, int y, int width, int height){
	EM_ASM_({
		contexts[$0].fillRect($1, $2, $3, $4);
	}, ctxNum, x, y, width, height);
}

void beginPath(int ctxNum){
	EM_ASM_({
		contexts[$0].beginPath();
	}, ctxNum);
}

void arc(int ctxNum, int x, int y, int radius){
	EM_ASM_({
		contexts[$0].arc($1, $2, $3, 0, 2*Math.PI);
	}, ctxNum, x, y, radius);
}

void arcTo(int ctxNum, int x1, int y1, int x2, int y2, int radius){
	EM_ASM({
		contexts[$0].arcTo($1, $2, $3, $4, $5);
	}, ctxNum, x1, y1, x2, y2, radius);
}

void ellipse(int ctxNum, int x, int y, int radiusX, int radiusY, float rotation){
	EM_ASM({
		contexts[$0].ellipse($1, $2, $3, $4, $5, 0, 2*Math.PI);
	}, ctxNum, x, y, radiusX, radiusY, rotation);
}


void stroke(int ctxNum){
	EM_ASM_({
		contexts[$0].stroke();
	}, ctxNum);
}

void fill(int ctxNum){
	EM_ASM_({
		contexts[$0].fill();
	}, ctxNum);
}

void moveTo(int ctxNum, int x, int y){
	EM_ASM({
		contexts[$0].moveTo($1, $2);
	}, ctxNum, x, y);
}

void lineTo(int ctxNum, int x, int y){
	EM_ASM_({
		contexts[$0].lineTo($1, $2);
	}, ctxNum, x, y);
}

void closePath(int ctxNum){
	EM_ASM_({
		contexts[$0].closePath();
	}, ctxNum);
}

void drawLine(int ctxNum, int x1, int y1, int x2, int y2){
	beginPath(ctxNum);
	moveTo(ctxNum, x1, y1);
	lineTo(ctxNum, x2, y2);
	stroke(ctxNum);
}

void drawLineSeries(int ctxNum, int x, int y, std::vector<CoOrd> points){
	beginPath(ctxNum);
	moveTo(ctxNum, x + points[0].x, y + points[0].y);
	for(int i = 1; i < points.size(); i++){
		lineTo(ctxNum, x + points[i].x, y + points[i].y);
	}
	closePath(ctxNum);
	fill(ctxNum);
}

void setTextAlign(int ctxNum, std::string alignment){
	EM_ASM_({
		contexts[$0].textAlign = UTF8ToString($1);
	}, ctxNum, alignment.c_str());
}

void setTextBaseline(int ctxNum, std::string textBaseline){
	EM_ASM_({
		contexts[$0].textBaseline = UTF8ToString($1);
	}, ctxNum, textBaseline.c_str());
}

void setFontSize(int ctxNum, int fontSize){
	EM_ASM_({
		contexts[$0].font = $1 + "px 'Chewy', cursive";
	}, ctxNum, fontSize);
}

void setMiterLimit(int ctxNum, int miterLimit){
	EM_ASM_({
		contexts[$0].miterLimit = $1;
	}, ctxNum, miterLimit);
}

void setLineCap(int ctxNum, int lineCapCode){
	EM_ASM_({
		switch($1){
			case 0:
				contexts[$0].lineCap = "butt";
				break;
			case 1:
				contexts[$0].lineCap = "round";
				break;
			case 2:
				contexts[$0].lineCap = "square";
				break;
		}
	}, ctxNum, lineCapCode);
}

void setLineJoin(int ctxNum, int lineJoinCode){
	EM_ASM_({
		switch($1){
			case 0:
				contexts[$0].lineJoin = "bevel";
				break;
			case 1:
				contexts[$0].lineJoin = "round";
				break;
			case 2:
				contexts[$0].lineJoin = "miter";
				break;
		}
	}, ctxNum, lineJoinCode);
}

void strokeText(int ctxNum, std::string text, int x, int y){
	EM_ASM_({
		contexts[$0].strokeText(UTF8ToString($1), $2, $3);
	}, ctxNum, text.c_str(), x, y);
}

void fillText(int ctxNum, std::string text, int x, int y){
	EM_ASM_({
		contexts[$0].fillText(UTF8ToString($1), $2, $3);
	}, ctxNum, text.c_str(), x, y);
}

void drawCurvedRectangle(int ctxNum, int x, int y, int width, int height, int cornerRadius){
	if(cornerRadius < 1){
		return;
	}

	if(width < cornerRadius*2){
		width = cornerRadius*2;
	}
	beginPath(ctxNum);
	moveTo(ctxNum, x + cornerRadius, y);
	lineTo(ctxNum, x + width - cornerRadius, y);
	arcTo(ctxNum, x + width, y, x + width, y + cornerRadius, cornerRadius);
	lineTo(ctxNum, x + width, y + height - cornerRadius);
	arcTo(ctxNum, x + width, y + height, x + width - cornerRadius, y + height, cornerRadius);
	lineTo(ctxNum, x + cornerRadius, y + height);
	arcTo(ctxNum, x, y + height, x, y + height - cornerRadius, cornerRadius);
	lineTo(ctxNum, x, y + cornerRadius);
	arcTo(ctxNum, x, y, x + cornerRadius, y, cornerRadius);
	stroke(ctxNum);
}

void hideElement(std::string elementName){
	EM_ASM_({
		document.getElementById(UTF8ToString($0)).style.display = "none";
	}, elementName.c_str());
}

void showElement(std::string elementName){
	EM_ASM_({
		document.getElementById(UTF8ToString($0)).style.display = "initial";
	}, elementName.c_str());
}

void drawMap(){
	double mapStartX = 0;
	double mapStartY = 0;
	double mapEndX = MAX_MAP_WIDTH;
	double mapEndY = MAX_MAP_HEIGHT;

	Location topLeftGridCorner = {mapStartX, mapStartY};
	Location mapStart = camera.getRelativePosition(topLeftGridCorner);
	Location bottomRightGridCorner = {mapEndX, mapEndY};
	Location mapEnd = camera.getRelativePosition(bottomRightGridCorner);

	//Grid lines
	setLineWidth(0, 1);
	setStrokeStyle(0, 198, 198, 198);
	setGlobalAlpha(0, 1);

	//Horizontal Line Loop
	for(double h = mapStartY - 1000; h < mapEndY + 1000; h += 25){
		beginPath(0);
		moveTo(0, mapStart.x - 2000, mapStart.y + h);
		lineTo(0, mapEnd.x + 2000, mapStart.y + h);
		stroke(0);
	}
	//Vertical Line Loop
	for(double v = mapStartX - 2000; v < mapEndX + 2000; v += 25){
		beginPath(0);
		moveTo(0, mapStart.x + v, mapStart.y - 2000);
		lineTo(0, mapStart.x + v, mapEnd.y + 2000);
		stroke(0);
	}

	//Map utilities
	setLineWidth(0, 2);
	setFillStyle(0, 74, 74, 74);
	setStrokeStyle(0, 74, 74, 74);
	setFontSize(0, 12);
	setGlobalAlpha(0, 1);
	Player player = players.at(selfId);

	//Upgrade Shop
	Location upgradeShopStart = camera.getRelativePosition({600, -100});
	fillText(0, "Upgrades", upgradeShopStart.x + 3, upgradeShopStart.y + 10);
	if(player.x > 600 && player.y > -100 && player.x < 700 && player.y < 0){
		setStrokeStyle(0, 232, 196, 85);
	}
	drawCurvedRectangle(0, upgradeShopStart.x, upgradeShopStart.y, 100, 100, 2);

	//Fuel Station
	Location fuelStationStart = camera.getRelativePosition({1000, -100});
	fillText(0, "Refuel", fuelStationStart.x + 3, fuelStationStart.y + 10);
	setStrokeStyle(0, 74, 74, 74);
	if(player.x > 1000 && player.y > -100 && player.x < 1100 && player.y < 0){
		setStrokeStyle(0, 232, 196, 85);
	}
	drawCurvedRectangle(0, fuelStationStart.x, fuelStationStart.y, 100, 100, 2);

	//Market
	Location marketStart = camera.getRelativePosition({1400, -100});
	fillText(0, "Market", marketStart.x + 3, marketStart.y + 10);
	setStrokeStyle(0, 74, 74, 74);
	if(player.x > 1400 && player.y > -100 && player.x < 1500 && player.y < 0){
		setStrokeStyle(0, 232, 196, 85);
	}
	drawCurvedRectangle(0, marketStart.x, marketStart.y, 100, 100, 2);
}

void drawHud(){
	setFillStyle(0, 255, 0, 0);
	setStrokeStyle(0, 255, 0, 0);
	setFontSize(0, 12);
	setGlobalAlpha(0, 1);

	Player player = players.at(selfId);
	fillText(0, "Fuel: " + std::to_string(player.getFuel()) + "L", 3, 10);
	fillText(0, "Bank: $" + std::to_string(player.bank), 3, 22);
	fillText(0, "X: " + std::to_string(player.x), 3, 34);
	fillText(0, "Y: " + std::to_string(player.y), 3, 46);

	player.cargo.print();
}

void drawUserInterfaces(){
	Player* player = &players.at(selfId);
	if(player->insideUpgradeShop){
		setFillStyle(0, 173, 178, 186);
		setStrokeStyle(0, 173, 178, 186);
		setTextAlign(0, "center");
		setTextBaseline(0, "middle");
		drawCurvedRectangle(0, 100, 100, 250, 250, 5);
		fill(0);

		//Fuel Tank
		setFillStyle(0, 143, 147, 155);
		drawCurvedRectangle(0, 125, 125, 200, 50, 2);
		if(cursor.hudNew.x > 125 && cursor.hudNew.y > 125 && cursor.hudNew.x < 325 && cursor.hudNew.y < 175){
			setFillStyle(0, 142, 138, 138);
		}
		if(cursor.hudClick.x > 125 && cursor.hudClick.y > 125 && cursor.hudClick.x < 325 && cursor.hudClick.y < 175){
			player->upgrade(0);
		}
		fill(0);
		setFillStyle(0, 74, 74, 74);
		fillText(0, "Upgrade fuel tank", 225, 150);

		//Drill
		setFillStyle(0, 143, 147, 155);
		drawCurvedRectangle(0, 125, 200, 200, 50, 2);
		if(cursor.hudNew.x > 125 && cursor.hudNew.y > 200 && cursor.hudNew.x < 325 && cursor.hudNew.y < 250){
			setFillStyle(0, 142, 138, 138);
		}
		if(cursor.hudClick.x > 125 && cursor.hudClick.y > 200 && cursor.hudClick.x < 325 && cursor.hudClick.y < 250){
			player->upgrade(1);
		}
		fill(0);
		setFillStyle(0, 74, 74, 74);
		fillText(0, "Upgrade drill", 225, 225);
	}
	setTextAlign(0, "start");
	setTextBaseline(0, "alphabetic");
}

void drawDebugVariables(){
	
}