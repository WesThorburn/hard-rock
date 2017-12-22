#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>
#include "coOrd.h"

std::vector<std::string> split(char charArray[], char delimiter);
int getClientTime();
int getLerpIncrement(int currentValue, int trueValue);
std::string convertToKMB(int number); //Converts 1000 to 1k, same with millions and billions
std::vector<CoOrd> convertToCoOrds(std::vector<std::vector<float>> distanceAngle, int angle); //Converts distance/angle into x,y

#endif