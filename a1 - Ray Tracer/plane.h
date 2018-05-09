#ifndef _PLANE_H
#define _PLANE_H

#include "vect.h"
#include "color.h"
#include "math.h"
#include "object.h"

class plane : public object{
	vect normal;
	double distance;
	color colour;

public:
	plane();
	plane(vect,double,color);

	vect getPlaneNormal(){
		return normal;
	}

	double getPlaneDistance(){
		return distance;
	}

	virtual color getColor(){
		return colour;
	}

	virtual vect getNormalAt(vect point){
		return normal;
	}

	virtual double findIntersection(ray r){
		vect r_direction = r.getRayDirection();
		double a = r_direction.dotProduct(normal);
		//ray is parallel to plane
		if(a==0){
			return -1;
		}else{
			double b = normal.dotProduct(r.getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			return -1*b/a; //distance from ray origin to point of intersection
		}
	}
};

plane::plane(){
	normal = vect(1,0,0);
	distance = 0.0;
	colour = color(0.3,0.2,0.8,0);
}

plane::plane(vect normalValue, double distanceValue, color colourValue){
	normal = normalValue;
	distance = distanceValue;
	colour = colourValue;
}

#endif