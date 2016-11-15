//Wai Phyo
//Robert Harris
//CS256-HW5
//Completed on 007/2016


#ifndef PARTICLE_H
#define PARTICLE_H
#include "Point.h"
class Particle
{
private:
	Point board;
	Point velocity;
	double mass;
	double radius;
public:
	Particle();
	Particle(Point& inPoint, double inMass, double inRadius, Point& inVelocity);
	Particle(Point& inPoint, double inMass);
	Particle(const Particle& other);
	Particle& operator=(const Particle& right);
	~Particle();
	Point getPos() const;
	Point getVelocity() const;
	double getMass() const;
	double getRadius() const;
	void setPos(Point inPoint);
	void setVelocity(Point inPoint);
	void setMass(double inMass);
	void setRadius(double inRad);
	
};

#endif
