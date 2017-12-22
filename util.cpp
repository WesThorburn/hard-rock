#include <math.h>
#include <emscripten.h>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "util.h"

std::vector<std::string> split(char charArray[], char delimiter){
	std::vector<std::string> splitString;
	std::string messageString = std::string(charArray);
	std::stringstream ss;
	ss.str(messageString);
	std::string item;
	while(getline(ss, item, delimiter)){
		splitString.push_back(item);
	}
	return splitString;
}

int getClientTime(){
	return EM_ASM_INT({
		return new Date().getTime();
	});
}

int getLerpIncrement(int currentValue, int trueValue){
	if(currentValue < trueValue - 5){
		return 5;
	}
	else if(currentValue < trueValue - 3){
		return 3;
	}
	else if(currentValue < trueValue){
		return 1;
	}
	else if(currentValue > trueValue + 5){
		return -5;
	}
	else if(currentValue > trueValue + 3){
		return -3;
	}
	else if(currentValue > trueValue){
		return -1;
	}
	return 0;
}

std::string divideAndSetPrecision(int number, int divisor, int precision){
	float formattedNumber = (float)number/(float)divisor;
	std::stringstream ss;
	ss << std::fixed << std::setprecision(precision) << formattedNumber;
	return ss.str();
}

std::string convertToKMB(int number){
	if(number < 1000){
		return std::to_string(number);
	}
	else if(number >= 1000 && number < 1000000){
		return divideAndSetPrecision(number, 1000, 1) + "K";
	}
	else if(number >= 1000000 && number < 1000000000){
		return divideAndSetPrecision(number, 1000000, 1) + "M";
	}
	else if(number >= 1000000000){
		return divideAndSetPrecision(number, 1000000000, 1) + "B";
	}
	return std::to_string(number);
}

//Converts distance/angle vector into x,y co-ords based on the angle
std::vector<CoOrd> convertToCoOrds(std::vector<std::vector<float>> distanceAngle, int angle){
	float angleRadians = angle * M_PI/180;

	std::vector<CoOrd> coOrdinates;
	for(int i = 0; i < distanceAngle.size(); i++){
		CoOrd point;
		point.x = distanceAngle.at(i)[0] * cos(distanceAngle.at(i)[1] + angleRadians);
		point.y = distanceAngle.at(i)[0] * sin(distanceAngle.at(i)[1] + angleRadians);
		coOrdinates.push_back(point);
	}
	return coOrdinates;
}