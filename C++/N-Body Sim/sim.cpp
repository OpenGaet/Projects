#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <random>
#include "Body.h"


void update(int bodyIndex, Body** bodies, int nBod)
{
	int j;
	
	for (j = bodyIndex + 1; j < nBod && j < 1000; j++)
	{
		bodies[bodyIndex]->calcGrav(bodies[j]);
	}
	
	bodies[bodyIndex]->calcAccel();
	bodies[bodyIndex]->calcVel();
	bodies[bodyIndex]->calcPos();
}


 void sim(int nBod, int sec) //Body**
{
	srand(static_cast <unsigned> (time(0)));

	double mass;
	double xp;
	double yp;
	double zp;
	double xv;
	double yv;
	double zv;
	double LO_MASS = 3.0 * pow(10, 23); // mass of mercury
	double HI_MASS = 2.0 * pow(10, 30); // mass of sun
	double LO_POS = 0;
	double HI_POS = 150.0 * pow(10, 9); // dist from sun to earth
	double LO_VEL = 0.0;
	double HI_VEL = 3.0 * pow(10, 2); // 0.0001% c

	// Limit for number of bodies is 1000
	Body* bodies[1000];

	// Create data file
	std::ofstream myFile;
	myFile.open("data.dat");
	myFile << std::fixed;

	// Large number random generation
	// Seed
	std::random_device r;
	// Random number generator
	std::default_random_engine re(r());
	// Distribution on which to apply the generator
	std::uniform_real_distribution<double> distributionPos(LO_POS, HI_POS); 
	std::uniform_real_distribution<double> distributionMass(LO_MASS, HI_MASS);
	std::uniform_real_distribution<double> distributionVel(LO_VEL, HI_VEL);

	for (int i = 0; i < nBod && i < 1000; i++)
	{
		mass = distributionMass(re); 
		xp = distributionPos(re); 
		yp = distributionPos(re);
		xv = distributionVel(re); 
		yv = distributionVel(re);

		// Write positions to data file
		bodies[i] = new Body(mass, xp, yp, 0.0, xv, yv, 0.0);
		myFile << bodies[i]->getXpos() << '\t' << bodies[i]->getYpos() << std::endl;
	}
	myFile << std::endl << std::endl;

	//return bodies;
	int frames = 60 * sec;
	for (int i = 0; i < frames - 1; i++)
	{
		for (int j = 0; j < nBod && j < 1000; j++)
		{
			update(j, bodies, nBod);
			myFile << bodies[j]->getXpos() << '\t' << bodies[j]->getYpos() << std::endl;
		}
		myFile << std::endl << std::endl;
	}

	myFile.close();
}


int main()
{
	const int nBod = 500;
	const int sec = 10;
	sim(nBod,sec);

	return 0;
}