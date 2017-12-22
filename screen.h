#ifndef screen_H
#define screen_H

#include <string>
#include "camera.h"

struct Browser{ int width; int height; };
struct AspectRatio{ float optimal; float current; };
struct Scale{ float currentX; float currentY; };
struct RenderArea{ int canvasWidth; int canvasHeight; int hudWidth; int hudHeight; };

extern Browser browser;
extern AspectRatio aspectRatio;
extern Scale canvasScale;
extern Scale hudScale;
extern Camera camera;
extern RenderArea renderArea;

void buildCanvases();
void clearCanvas(int ctxNum);
void scaleCanvas(int ctxNum, float scaleX, float scaleY);
void setTransform(int ctxNum, double hScale, double hSkew, double vSkew, double vScale, double hMove, double vMove);
void setBrowserSize(int cameraOnly = 0);
double getDevicePixelRatio();
void setCanvasSize(std::string elementName);

#endif