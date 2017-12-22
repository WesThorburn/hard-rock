#include <emscripten.h>
#include <emscripten/bind.h>
#include <math.h>
#include <iostream>
#include "screen.h"
#include "drawing.h"
#include "player.h"
#include "variables.h"

Browser browser;
AspectRatio aspectRatio;
Scale canvasScale;
Scale hudScale;
Camera camera;
RenderArea renderArea;

void buildCanvases(){
	EM_ASM(
		var ctx = document.getElementById("canvas").getContext("2d");
		contexts[0] = ctx;

		var hud = document.getElementById("hud").getContext("2d");
		contexts[1] = hud;

		Module.setCanvasSize("canvas");

		window.onresize = function(){
			Module.setBrowserSize(0);
		}
	);
	setBrowserSize(0);
}

void clearCanvas(int ctxNum){
	if(ctxNum == 1){ //Hud uses clearRect
		EM_ASM_({
			contexts[$0].clearRect(0, 0, $1, $2);
		}, ctxNum, renderArea.hudWidth, renderArea.hudHeight);
	}
	else{
		setGlobalAlpha(0, 1);
		setFillStyle(0, 214, 214, 214);
		EM_ASM_({
			contexts[$0].fillRect(0, 0, $1, $2);
		}, ctxNum, renderArea.canvasWidth, renderArea.canvasHeight);
	}
}

void scaleCanvas(int ctxNum, float scaleX, float scaleY){
	EM_ASM({
  		contexts[$0].scale($1, $2);
  	}, ctxNum, scaleX, scaleY);
}

void setTransform(int ctxNum, double hScale, double hSkew, double vSkew, double vScale, double hMove, double vMove){
	EM_ASM({
		contexts[$0].setTransform($1, $2, $3, $4, $5, $6);
	}, ctxNum, hScale, hSkew, vSkew, vScale, hMove, vMove);
}

void setBrowserSize(int cameraOnly){//cameraOnly is used when increasing view range
	/*toResize = 1;
	if(resizeWaitTimer < 10){
		return;
	}
	resizeWaitTimer = 0;
	toResize = 0;*/

	double devicePixelRatio = getDevicePixelRatio();

	int width = EM_ASM_INT({
		return window.innerWidth;
	});

	int height = EM_ASM_INT({
		return window.innerHeight;
	});

	setCanvasSize("canvas");

	browser.width = width;
	browser.height = height;

	if(camera.width <= 0 || camera.height <= 0 || browser.width <= 0 || browser.height <= 0){
		return;
	}

	aspectRatio.optimal = (float)camera.width / (float)camera.height;
	aspectRatio.current = (float)browser.width / (float)browser.height;

	canvasScale.currentX = (float)browser.width / (float)camera.width;
	canvasScale.currentY = (float)browser.height / (float)camera.height;

	if(aspectRatio.current > aspectRatio.optimal){
		canvasScale.currentY = canvasScale.currentX;
	}
	else if(aspectRatio.current < aspectRatio.optimal){
		canvasScale.currentX = canvasScale.currentY;
	}

	camera.windowSizeOffset.x = round(browser.width / canvasScale.currentX / 2);
	camera.windowSizeOffset.y = round(browser.height / canvasScale.currentY / 2);

	if(camera.trackingId >= 0){
		Player trackingPlayer = players.at(camera.trackingId);
		camera.x = trackingPlayer.x - camera.windowSizeOffset.x;
		camera.y = trackingPlayer.y - camera.windowSizeOffset.y;
	}

	renderArea.canvasWidth = round(browser.width / canvasScale.currentX);
	renderArea.canvasHeight = round(browser.height / canvasScale.currentY);

	scaleCanvas(0, devicePixelRatio, devicePixelRatio);
	scaleCanvas(0, canvasScale.currentX, canvasScale.currentY);

	EM_ASM_({
		renderArea.canvasWidth = $2;
		renderArea.canvasHeight = $3;
	}, canvasScale.currentX, canvasScale.currentY, renderArea.canvasWidth, renderArea.canvasHeight);

	if(!cameraOnly){
		hudScale.currentX = canvasScale.currentX;
		hudScale.currentY = canvasScale.currentY;

		renderArea.hudWidth = round(browser.width / hudScale.currentX);
		renderArea.hudHeight = round(browser.height / hudScale.currentY);

		setCanvasSize("hud");
		scaleCanvas(1, devicePixelRatio, devicePixelRatio);
		scaleCanvas(1, canvasScale.currentX, canvasScale.currentY);
	}
}

EMSCRIPTEN_BINDINGS(setBrowserSize){
	emscripten::function("setBrowserSize", &setBrowserSize);
};

double getDevicePixelRatio(){
	return EM_ASM_DOUBLE({
		return window.devicePixelRatio;
	});
}

void setCanvasSize(std::string elementName){
	EM_ASM_({
		var canvas = document.getElementById(UTF8ToString($0));
		canvas.width = window.innerWidth * window.devicePixelRatio;
		canvas.height = window.innerHeight * window.devicePixelRatio;
		canvas.style.width = canvas.width / window.devicePixelRatio + "px";
		canvas.style.height = canvas.height / window.devicePixelRatio + "px";
	}, elementName.c_str());
}

EMSCRIPTEN_BINDINGS(setCanvasSize){
	emscripten::function("setCanvasSize", &setCanvasSize);
}