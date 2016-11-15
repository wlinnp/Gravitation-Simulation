//Wai Phyo
//Robert Harris
//CS256-HW5
//Completed on 007/2016


#include "Particle.h"
#include "Constants.h"

Particle::Particle():board(0,0),  velocity(0,0), mass(0), radius(0){}
Particle::Particle(Point& inPoint, double inMass, double inRadius,  Point& inVelocity):board(inPoint), velocity(inVelocity), mass(inMass), radius(inRadius) {}
Particle::Particle(Point& inPoint, double inMass):board(inPoint), velocity(0,0), mass(inMass), radius( (MAX_SIZE * inMass / MAX_MASS) + MIN_SIZE ) {}
Particle::Particle(const Particle& other):board(other.board), velocity(other.velocity), mass(other.mass), radius(other.radius) {}
Particle& Particle::operator=(const Particle& right)
{
	if (this != &right)
	{
		board = right.board;
		radius = right.radius;
		mass = right.mass;
		velocity = right.velocity;
	}
	return *this;
}
Particle::~Particle()	{}
Point Particle::getPos() const
{
	return board;
}
Point Particle::getVelocity() const
{
	return velocity;
}
double Particle::getMass() const
{
	return mass;
}
double Particle::getRadius() const
{
	return radius;
}
void Particle::setPos(Point inPoint)
{
	board = inPoint;
}
void Particle::setVelocity(Point inVelo)
{
	velocity = inVelo;
}
void Particle::setMass(double inMass)
{
	mass = inMass;
}
void Particle::setRadius(double inRad)
{
	radius = inRad;
}
