//Wai Phyo
//Robert Harris
//CS256-HW5
//Completed on 007/2016

#include "Point.h"

Point::Point():x(0),y(0) {}

Point::Point(double inX, double inY):x(inX), y(inY) {}

Point::Point(const Point& other):x(other.x),y(other.y) {}
Point& Point::operator=(const Point& right)
{
	x = right.x;
	y = right.y;
	return *this;
}

Point::~Point() {}

double Point::getX() const {	return x;	}

double Point::getY() const {	return y;	}

void Point::setX(double val) {	x = val;	}

void Point::setY(double val) {	y = val;	}
