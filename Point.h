//Wai Phyo
//Robert Harris
//CS256-HW5
//Completed on 007/2016


#ifndef POINT_H
#define POINT_H
class Point
{
private:
	double x;// x position
	double y;// y position
public:
	Point();
	Point(double inX, double inY);
	Point(const Point& other);
	Point& operator=(const Point& right);
	~Point();
	double getX() const;
	double getY() const;
	void setX(double val);
	void setY(double val);
};
#endif
