#include "particle.h"

/**
*Default constructor
*/

 particle::particle(double a, double b, double mass){
     this->x=a;
     this->y=b;
     this->m=mass;
     ax=0;
     ay=0;
     vx=0;
     vy=0;
 }

 /**
*Utility functions
*/

 double particle::X(){
     return this->x;
 }

  double particle::Y(){
     return this->y;
 }

  double particle::M(){
     return this->m;
 }

  double particle::VX(){
     return this->vx;
 }

  double particle::VY(){
     return this->vy;
 }

  double particle::AX(){
     return this->ax;
 }

  double particle::AY(){
     return this->ay;
 }

/**
*Function to add components to acceleration
*@param a: x component
*@param b: y component
*/

 void particle::newAcc(double a, double b){
    (this->ax)+=a;
    (this->ay)+=b;
    return;
 }

/**
*Set acceleration to zero
*/

  void particle::reset(){
    (this->x)=0;
    (this->y)=0;
    (this->m)=0;
    return;
  }

  void particle::resetForce(){
    (this->ax)=0;
    (this->ay)=0;
    return;
 }
 
 void particle::setM(double mass){
 	(this->m)+=mass;
 	return;
 }
 
  void particle::setX(double a){
 	(this->x)+=a;
 	return;
 }
 
  void particle::setY(double b){
 	(this->y)+=b;
 	return;
 }

/**
*Change velocity using updated force
*This is only an approximation, integration would be more appropriate
*Also max capping the velocities for realistic simulation
*/

 void particle::newVel(){
    (this->vx)+=0.05*(this->ax);
    //this->vx=std::min(this->vx,20.0);
    //this->vx=std::max(this->vx,-20.0);
    (this->vy)+=0.05*(this->ay);
    //this->vy=std::min(this->vy,20.0);
    //this->vy=std::max(this->vy,-20.0);

     return;
 }

/**
*Approximate motion using force and old velocity
*Time units can be scaled to change speed of evolution 
*/

 void particle::newLoc(){
     (this->x)+=(0.1*(this->vx)+0.005*(this->ax))*0.05;
     (this->y)+=(0.1*(this->vy)+0.005*(this->ay))*0.05;
     return;
     
 }
