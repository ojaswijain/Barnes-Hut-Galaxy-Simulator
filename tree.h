#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include "particle.h"

using std::vector;

/**
*Class-based representation of the quad tree
*A single instance of the class represents a single patch of the screen, containing a single particle
*Division of the tree is recursive
*/

class tree{

    private:

        /**
        *Parameters of the quadtree patch
        *@param left: x-coordinate of the left edge
        *@param right: x-coordinate of the right edge
        *@param top: y-coordinate of the top edge
        *@param bottom: y-coordinate of the bottom edge
        *@param theta: critical ratio for the Barnes-Hut algorithm
        *@param size: side length of the patch
        */

        double left;
        double right;
        double top;
        double bottom;
        double theta;
        int size;

         /**
        *@param CoM: representative particle of the patch
        *@param children: vector containing the 4 children of the patch 
        *@param parent:to check if the patch has non-null child patches or not
        */

        particle* CoM;
        tree* children[4];
        bool parent;

    public:

        /**
        *Constructor and destructor
        */

        tree(double l, double r, double t, double b, particle* p);
        ~tree();

        /**
        *Functions to add particles, and update accelerations
        *Implementations for both single particle and multiple particles
        */

        void clear();
        void add (particle* p);
        void add (vector <particle*> p);
        void updateAcc(particle* p);

        /**
        *Functions involving the CoM of the patch
        *Functions to designate child patches as well
        */

        particle* node();
        void updateNode();
        int getQuad(particle* p);

        /**
        *Utility functions to retrieve private variables
        */

        double L();
        double R();
        double T();
        double B();
        double Theta();
        bool isParent();

        /**
        *Functions to access children nodes
        *Also generate new nodes as and when required
        */

        tree* getChild(int idx);
        void makeChildren();

};

#endif
