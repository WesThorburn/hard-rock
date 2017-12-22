#ifndef CONTROLS_H
#define CONTROLS_H

#include "location.h"

struct Cursor{
	Location canvasNew;
	Location canvasOld;
	Location canvasClick;
	Location hudNew;
	Location hudClick;
};

extern Cursor cursor;

void setupControls();
void toggleDebug();
void emitCursorData();

#endif