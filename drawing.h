#ifndef DRAWING_H
#define DRAWING_H

#include <string>
#include <vector>
#include "coOrd.h"
#include "player.h"

void setFillStyle(int ctxNum, int r, int g, int b);
void setStrokeStyle(int ctxNum, int r, int g, int b);
void setGlobalAlpha(int ctxNum, float globalAlpha);
void setLineWidth(int ctxNum, int lineWidth);
void setCursorPointer();
void setCursorDefault();
void drawRectangle(int ctxNum, int x, int y, int width, int height);
void beginPath(int ctxNum);
void arc(int ctxNum, int x, int y, int radius);
void arcTo(int ctxNum, int x1, int y1, int x2, int y2, int radius);
void ellipse(int ctxNum, int x, int y, int radiusX, int radiusY, float rotation);
void stroke(int ctxNum);
void fill(int ctxNum);
void moveTo(int ctxNum, int x, int y);
void lineTo(int ctxNum, int x, int y);
void closePath(int ctxNum);
void drawLine(int ctxNum, int x1, int x2, int y1, int y2);
void drawLineSeries(int ctxNum, int x, int y, std::vector<CoOrd> points);
void setTextAlign(int ctxNum, std::string alignment);
void setTextBaseline(int ctxNum, std::string textBaseline);
void setFontSize(int ctxNum, int fontSize);
void setMiterLimit(int ctxNum, int miterLimit);
void setLineCap(int ctxNum, int lineCapCode);
void setLineJoin(int ctxNum, int lineJoinCode);
void strokeText(int ctxNum, std::string text, int x, int y);
void fillText(int ctxNum, std::string text, int x, int y);
void drawCurvedRectangle(int ctxNum, int x, int y, int width, int height, int cornerRadius);
void hideElement(std::string elementName);
void showElement(std::string elementName);
void drawMap();
void drawHud();
void drawUserInterfaces();
void drawUpgradeInterface(Player* player);
void drawDebugVariables();

#endif
