#include <iostream>
#include <emscripten.h>
#include <emscripten/bind.h>
#include "game.h"
#include "screen.h"
#include "variables.h"
#include "drawing.h"

void enterGame(){
	setBrowserSize(0);
}
EMSCRIPTEN_BINDINGS(enterGame){
	emscripten::function("enterGame", &enterGame);
}

void checkAdBlocking(){
	EM_ASM({
		if(window.runningAds === undefined){
			Module.ab();
		}
	});
}

void ab(){
	//send("x,1");
	//notifications.push_back(Notification({"r", "7", "Please consider disabling your adblocker."}));
}
EMSCRIPTEN_BINDINGS(ab){
	emscripten::function("ab", &ab);
}

void resetObjectVectors(){
	
}