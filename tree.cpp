#include "tree.h"
#include <cmath>
#include <algorithm>
using std::swap;
using std::min;

//value for double comparisions
double thresh=1e-6;
double alpha=10;

/**
*Default constructor
*/

tree::tree(double l, double r, double t, double b, particle* p){

    this->left=l;
    this->right=r;
    this->top=t;
    this->bottom=b;
    this->parent=false;
    this->theta=0.5;
    this->CoM=p;
    this->size=abs(l-r);

	if(this->left > this->right){
		swap(this->left, this->right);
    }
	if(this->bottom > this->top){
		swap(this->bottom, this->top);
    }
    
    for(int i=0;i<4;i++){
    	children[i]=NULL;
	}
}

/**
*Default destructor utility function
*/

void tree::clear(){

	if(this->parent=false){
		this->CoM = NULL;
		delete this->CoM;
		return;
	}

	for(int i=0;i<4;i++){
		auto c=this->children[i];
		if( c == NULL ){
			continue;
		}
		c->clear();
		c = NULL;
		delete c;
	}
	this->parent = false;
	this->CoM = NULL;
	delete this->CoM;
}

/**
*Default destructor
*/

tree::~tree(){
    this->clear();
}

/**
*Function to add a single particle 
*Recurses till the exact quadrant is found
*/

void tree::add (particle* p){
    //if null particle added, or particle not in quadrant
    if(p==NULL||this->getQuad(p)>3){
        return;
    }

    //if patch already empty
    if(this->CoM==NULL){
        this->CoM=p;
        return;
    }

    //if patch full, divide the quadtree
    if(this->parent==false){
        this->makeChildren();

        this->children[this->getQuad(p)]->add(p);
		this->children[this->getQuad(this->CoM)]->add(this->CoM);

        //update the CoM of the patch
        this->CoM = new particle(0, 0, 0);
		this->updateNode();
		return;
	}
	
	this->children[this->getQuad(p)]->add(p);
	this->updateNode();
}

/**
*Function to add a multiple particles 
*/

void tree::add (vector<particle*> p){
    for (auto x:p){
        this->add(x);
    }
}

/**
 * the key function of the algorithm 
 * Calculates forces between all particles by bunching together faraway particles
 * N log(N) time complexity
 * Scaling of metrics to visualise the motion properly
 */

void tree::updateAcc(particle* p){
    //if any of the two has zero mass, or both are same particles
    if((this->CoM == NULL)||(p == NULL)||(this->CoM==p)){
		return;
	}

    //applying gravitational force formula
	double dx = (this->CoM->X())-(p->X());
	double dy = (this->CoM->Y())-(p->Y());
	double d2 = dx*dx + dy*dy;
	double d = sqrt(d2);
	double d3 = d*d*d;

	if(this->parent==false){
		if(fabs(this->CoM->M()) < thresh){
			return;
		}
		double m2 = this->CoM->M();
		double a = alpha*dx*m2/(abs(d3)+0.2);
		double b = alpha*dy*m2/(abs(d3)+0.2);
        p->newAcc(a,b);
		return;
	}

	double s = this->size;

    //checking for the critical condition and edge cases
	if(( (s/d) >= this->theta )||( this->getQuad(p)<=3)){
        for(int i=0;i<4;i++){
        	auto x=this->children[i];
            x->updateAcc(p);
        }
		return;
	}

	else{
		double m2 = this->CoM->M();
		double a = alpha*dx*m2/(abs(d3)+0.2);
		double b = alpha*dy*m2/(abs(d3)+0.2);

        p->newAcc(a,b);
		return;
	}

}

/**
*Function to retrieve CoM of patch 
*/

particle* tree::node(){
    return this->CoM;
}

/**
*Function to change the metrics of node after adding new particle 
*/

void tree::updateNode(){

    if(this->parent==false){
		return;
    }

	this->CoM->reset();

	for(int i=0;i<4;i++){
		auto x= this->children[i];
		double mass=0;
		if( x->node()!=NULL){
			mass += x->node()->M();
        }
        this->CoM->setM(mass);
	}

	if(fabs( this->CoM->M() ) < thresh){
		return;
    }

	particle* child = NULL;
	for(int i=0;i<4;i++){
		auto x= this->children[i];
		child = x->node();
		double a=0,b=0;
		if( child != NULL){
			a += child->X()*child->M();
			b += child->Y()*child->M();
		}
		double mass=this->CoM->M();
		this->CoM->setX(a/mass);
		this->CoM->setY(b/mass);
	}
}

/**
*Function to get the quadrant of the particle
*indices of respective quadrants explained in comments
*/

int tree::getQuad(particle* p){

    //if non existent particle
    if( p == NULL )
		return 4;

    //if not in quadrant
	if(( p->X() < this->left ) || ( p->X() >= this->right ) || ( p->Y() < this->bottom ) || ( p->Y() >= this->top ))
		return 4;

    //
	if(p->X() < (this->left + this->right)/2.0){
		if(p->Y() < (this->top + this->bottom)/2.0){
            //top left
			return 2;
        }
        //bottom left
		else return 1;
	}
	else{
		if( p->Y() < (this->top + this->bottom)/2.0){
            //top right
			return 3;
        }
        //bottom right
		else return 0;
	}

	return 4;
}

/**
*Utility functions
*Return the private parameters 
*/

double tree::L(){
    return this->left;
}

double tree::R(){
    return this->right;
}

double tree::T(){
    return this->top;
}

double tree::B(){
    return this->bottom;
}

double tree::Theta(){
    return this->theta;
}

bool tree::isParent(){
    return this->parent;
}

/**
*Function to return the subtree
*@param idx: index of the subtree 
*/

tree* tree::getChild(int idx){
    return this->children[idx];
}

/**
*Function to cut up the quadtree patch
*The indices used are as per the getQuad function  
*/

void tree::makeChildren(){

    if(this->CoM == NULL){
		return;
    }

	if(this->parent==true){
		for(int i=0;i<4;i++){
			auto x= this->children[i];
			delete x;
			x = NULL;
		}
	}

	double midX = (this->left + this->right)/2.0;
	double midY = (this->bottom + this->top)/2.0;

	this->children[0]=(new tree(midX, this->right, midY, this->top, NULL)); 
    this->children[1]=(new tree(this->left, midX, midY, this->top, NULL));
    this->children[2]=(new tree(this->left,midX,this->bottom, midY, NULL)); 
    this->children[3]=(new tree(midX, this->right,this->bottom, midY, NULL));

	this->parent = true;
}
