#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <random>
#include "Body.h"
#include "matplotlibcpp.h"


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

	int i, j;

	double mass;
	double xp;
	double yp;
	double zp;
	double xv;
	double yv;
	double zv;

	double LO_MASS = 3.0 * pow(10, 23); // mass of mercury 23
	double HI_MASS = 2.0 * pow(10, 30); // mass of sun 30
	double LO_POS = -6.0 * pow(10, 10); // 139.82 * pow(10, 6); // diameter of jupiter
	double HI_POS = 6.0 * pow(10, 10); // dist from sun to pluto 12
	double LO_VEL = 0.0;
	double HI_VEL = 3.0 * pow(10, 4); //4

	Body* bodies[1000];

	std::ofstream myFile;
	myFile.open("data.csv");

	// Large number random generation
	/* Seed */
	std::random_device r;
	/* Random number generator */
	std::default_random_engine re(r());
	/* Distribution on which to apply the generator */
	std::uniform_real_distribution<double> distributionPos(LO_POS, HI_POS); //roughly 3x the size of the solar system
	std::uniform_real_distribution<double> distributionMass(LO_MASS, HI_MASS); //mass of mercury to sun
	std::uniform_real_distribution<double> distributionVel(LO_VEL, HI_VEL); //speed of earth

	for (i = 0; i < nBod && i < 1000; i++)
	{
		myFile << "x" << i << "," << "y" << i << ",";
	}
	myFile << std::endl;
	
	for (i = 0; i < nBod && i < 1000; i++)
	{
		//bodies[i] = new Body();
		mass = distributionMass(re); // LO_MASS + (mult * (HI_MASS - LO_MASS));
		xp = distributionPos(re); //mult * HI_POS

		yp = distributionPos(re); //mult * HI_POS;

		xv = distributionVel(re); // LO_VEL + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (HI_VEL - LO_VEL)));
		yv = distributionVel(re); // LO_VEL + static_cast <double> (rand()) / (static_cast <double> (RAND_MAX / (HI_VEL - LO_VEL)));

		
		bodies[i] = new Body(mass, xp, yp, 0.0, xv, yv, 0.0);
		myFile << bodies[i]->getXpos() << "," << bodies[i]->getYpos() << ",";
	}

	myFile << std::endl;

	//return bodies;
	int frames = 60 * sec;
	for (i = 0; i < frames - 1; i++)
	{
		for (j = 0; j < nBod && j < 1000; j++)
		{
			update(j, bodies, nBod);
			myFile << bodies[j]->getXpos() << "," << bodies[j]->getYpos() << ",";
		}
		myFile << std::endl;
	}

	myFile.close();
}

 namespace plt = matplotlibcpp;

int main()
{
	sim(500, 5);



	return 0;
}