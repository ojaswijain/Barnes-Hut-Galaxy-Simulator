#ifndef PARTICLE_H
#define PARTICLE_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

/**
*Class-based representation of a single particle in the system.
*Can also be used to repesent a quadtree's Centre of Mass
*/

class particle{

    private:

        /**
        *Parameters of the particle
        *@param x: x-coordinate of the particle
        *@param y: y-coordinate of the particle
        *@param m: mass of the particle
        *@param vx: x-component of the velocity
        *@param vy: y-component of the velocity
        *@param ax: x-component of the acceleration
        *@param ay: y-component of the acceleration
        */

        double x,y;
        double m;
        double vx,vy;
        double ax,ay;

    public:

        /**
        *Constructor
        */

        particle(double a,double b,double mass);

        /**
        *Utility functions to return private variables
        */

        double X();
        double Y();
        double M();
        double VX();
        double VY();
        double AX();
        double AY();

        /**
        *Functions to update acceleration, velocity and locations
        */
		
		void setM(double mass);
		void setX(double a);
		void setY(double b);
        void newAcc(double a,double b);
        void reset();
        void resetForce();
        void newVel();
        void newLoc();
};

#endif
