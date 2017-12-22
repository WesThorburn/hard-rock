#include <emscripten.h>
#include <emscripten/bind.h>
#include <iostream>
#include <math.h>
#include "controls.h"
#include "variables.h"
#include "screen.h"

Cursor cursor;

void setupControls(){
	EM_ASM(
		document.onkeydown = function(event){
			Module.onkeydown(event.keyCode);
		};
		document.onkeyup = function(event){
			Module.onkeyup(event.keyCode);
		};
		document.onmousemove = function(event){
			Module.onmousemove(event.clientX, event.clientY);
		};
		document.onmousedown = function(event){
			Module.onmousedown(event.clientX, event.clientY);
		};
		document.onmouseup = function(event){
			Module.onmouseup(event.clientX, event.clientY);
		};
		document.onwheel = function(event){
			event.preventDefault();
			Module.onwheel(event.deltaY);
		};
		window.onblur = function(){
			Module.focus(0);
		};
		window.onfocus = function(){
			Module.focus(1);
		};
	);
}

void onkeydown(int keyCode){
	Player* player = &players.at(selfId);
	std::string keyDownMessage = "";
	switch(keyCode){
		case 65: //a
			keyDownMessage = "k,0,1";
			break;
		case 68: //d
			keyDownMessage = "k,1,1";
			break;
		case 87: //w
			keyDownMessage = "k,2,1";
			player->pressingForward = 1;
			break;
		case 83: //s
			keyDownMessage = "k,3,1";
			player->pressingBackward = 1;
			break;
		case 37: //left
			keyDownMessage = "k,0,1";
			break;
		case 39: //right
			keyDownMessage = "k,1,1";
			break;
		case 38: //up
			keyDownMessage = "k,2,1";
			break;
		case 40: //down
			keyDownMessage = "k,3,1";
			break;
		case 49: //1
			keyDownMessage = "k,8,1";
			break;
		case 50: //2
			keyDownMessage = "k,8,2";
			break;
		case 51: //3
			keyDownMessage = "k,8,3";
			break;
		case 52: //4
			keyDownMessage = "k,8,4";
			break;
		case 53: //5
			keyDownMessage = "k,8,5";
			break;
	}
	
}
EMSCRIPTEN_BINDINGS(onkeydown){
	emscripten::function("onkeydown", &onkeydown);
}

void onkeyup(int keyCode){
	Player* player = &players.at(selfId);
	std::string keyUpMessage = "";
	switch(keyCode){
		case 65: //a
			keyUpMessage = "k,0,0";
			break;
		case 68: //d
			keyUpMessage = "k,1,0";
			break;
		case 87: //w
			keyUpMessage = "k,2,0";
			player->pressingForward = 0;
			break;
		case 83: //s
			keyUpMessage = "k,3,0";
			player->pressingBackward = 0;
			break;
		case 37: //left
			keyUpMessage = "k,0,0";
			break;
		case 39: //right
			keyUpMessage = "k,1,0";
			break;
		case 38: //up
			keyUpMessage = "k,2,0";
			break;
		case 40: //down
			keyUpMessage = "k,3,0";
			break;
	}
	
}
EMSCRIPTEN_BINDINGS(onkeyup){
	emscripten::function("onkeyup", &onkeyup);
}

void onmousemove(int clientX, int clientY){
	cursor.canvasNew.x = clientX / canvasScale.currentX;
	cursor.canvasNew.y = clientY / canvasScale.currentY;

	cursor.hudNew.x = clientX / hudScale.currentX;
	cursor.hudNew.y = clientY / hudScale.currentY;
}
EMSCRIPTEN_BINDINGS(onmousemove){
	emscripten::function("onmousemove", &onmousemove);
}

void onmousedown(int clientX, int clientY){
	cursor.canvasClick.x = clientX / canvasScale.currentX;
	cursor.canvasClick.y = clientY / canvasScale.currentY;

	cursor.hudClick.x = clientX / hudScale.currentX;
	cursor.hudClick.y = clientY / hudScale.currentY;
}
EMSCRIPTEN_BINDINGS(onmousedown){
	emscripten::function("onmousedown", &onmousedown);
}

void onmouseup(int clientX, int clientY){
	//send("k,6,0");
	cursor.canvasClick.x = 0;
	cursor.canvasClick.y = 0;
	cursor.hudClick.x = 0;
	cursor.hudClick.y = 0;
	clickingHud = 0;
}
EMSCRIPTEN_BINDINGS(onmouseup){
	emscripten::function("onmouseup", &onmouseup);
}

void onwheel(int deltaY){
	if(selfId > 0){
		Player* player = &players.at(selfId);
		
	}
}
EMSCRIPTEN_BINDINGS(onwheel){
	emscripten::function("onwheel", &onwheel);
}

void focus(bool state){
	/*if(!connection.active){
		initiateConnection();
	}*/
	windowFocus = state;
	if(!windowFocus){
		//send("f");
	}
}
EMSCRIPTEN_BINDINGS(focus){
	emscripten::function("focus", &focus);
}

void toggleDebug(){
	debug = !debug;
}
EMSCRIPTEN_BINDINGS(toggleDebug){
    emscripten::function("toggleDebug", &toggleDebug);
}

void emitCursorData(){
	Player* player = &players.at(selfId);

	/*if(spectating || selfId <= 0){
		return;
	}*/
	//Check if cursor has moved since last update
	if(cursor.canvasNew.x != cursor.canvasOld.x || cursor.canvasNew.y != cursor.canvasOld.y){
		//Calculate relative cursor location
		Player* player = &players.at(selfId);
		Location playerRawPosition = {player->x, player->y};
		Location playerRelPos = camera.getRelativePosition(playerRawPosition);

		float mouseAngle = atan2(playerRelPos.y - cursor.canvasNew.y, playerRelPos.x - cursor.canvasNew.x) * 180/M_PI + 180;
		float playerAngle = round(mouseAngle + (asin(18/sqrt(pow((playerRelPos.x - cursor.canvasNew.x), 2) + pow((playerRelPos.y - cursor.canvasNew.y), 2)))) * 180/M_PI);

		int relativeCursorLocationX = round(playerRelPos.x - cursor.canvasNew.x);
		int relativeCursorLocationY = round(playerRelPos.y - cursor.canvasNew.y);

		if(playerAngle >= 360){
			playerAngle = playerAngle - 360;
		}
		else if(playerAngle < 0){
			playerAngle = 360 - playerAngle;
		}
		
		if(playerAngle >= 0 && playerAngle < 360){
			player->angle = round(playerAngle);
		}
	
		cursor.canvasOld.x = cursor.canvasNew.x;
		cursor.canvasOld.y = cursor.canvasNew.y;

		//Emit cursor data
		std::string cursorMessage = "m," + std::to_string(relativeCursorLocationX) + "," + std::to_string(relativeCursorLocationY) + "," + std::to_string(mouseAngle);
		//send(cursorMessage);
	}
}