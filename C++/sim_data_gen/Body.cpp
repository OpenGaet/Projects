#include "Body.h"
#include <math.h>


Body::Body(double mass, double xp, double yp, double zp, double xv, double yv, double zv)
{
	setMass(mass);
	setXpos(xp);
	setYpos(yp);
	setZpos(zp);
	setXvel(xv);
	setYvel(yv);
	setZvel(zv);
	setXgrav(0);
	setYgrav(0);
	setXacc(0);
	setYacc(0);
}

double Body::calcDist(double x1, double y1, double x2, double y2)
{
	double dist;
	dist = sqrt(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0));
	return dist;
}

void Body::calcGrav(Body* body)
{
	double m1 = this->getMass();
	double m2 = body->getMass();
	double GRAV_CONST = 6.674 * pow(10, -11); //-11 + 12 + 12
	double xDist = body->getXpos() - this->getXpos();
	double yDist = body->getYpos() - this->getYpos();
	//double dist = calcDist(this->getXpos(), this->getYpos(), body->getXpos(), body->getYpos());
	double gravFx = GRAV_CONST * m1 * m2 / pow(xDist, 2.0);
	double gravFy = GRAV_CONST * m1 * m2 / pow(yDist, 2.0);
	if (xDist < 0)
		gravFx *= -1;
	else if (yDist < 0)
		gravFy *= -1;
	this->gravFx += gravFx;
	this->gravFy += gravFy;
	body->setXgrav(body->getXgrav() + gravFx);
	body->setYgrav(body->getYgrav() + gravFy);
}

void Body::calcAccel()
{
	this->xacc = this->gravFx / this->getMass();
	this->yacc = this->gravFy / this->getMass();
}

void Body::calcVel()
{
	double time = 10.0;
	this->xvel = this->xvel + (this->xacc * (time));
	this->yvel = this->yvel + (this->yacc * (time));
}

void Body::calcPos()
{
	double time = 10.0;
	this->xpos = this->xpos + (this->xvel * (time)) + (0.5 * this->xacc * pow(time, 2.0));
	this->ypos = this->ypos + (this->yvel * (time)) + (0.5 * this->yacc * pow(time, 2.0));
}
