#ifndef _SPHERE_H
#define _SPHERE_H

#include "vect.h"
#include "color.h"
#include "math.h"
#include "object.h"

class sphere : public object{
	vect centre;
	double radius;
	color colour;

public:
	sphere();
	sphere(vect,double,color);

	vect getSphereCentre(){
		return centre;
	}

	double getSphereRadius(){
		return radius;
	}

	virtual color getColor(){
		return colour;
	}

	virtual vect getNormalAt(vect point){
		//normal away from centre of sphere
		vect normal_vect = point.vectAdd(centre.negative()).normalize();
		return normal_vect;
	}

	virtual double findIntersection(ray r){
		vect ray_origin = r.getRayOrigin();
		double ray_origin_x = ray_origin.getVectX();
		double ray_origin_y = ray_origin.getVectY();
		double ray_origin_z = ray_origin.getVectZ();

		vect ray_direction = r.getRayDirection();
		double ray_direction_x = ray_direction.getVectX();
		double ray_direction_y = ray_direction.getVectY();
		double ray_direction_z = ray_direction.getVectZ();

		vect sphere_centre = centre;
		double sphere_centre_x = sphere_centre.getVectX();
		double sphere_centre_y = sphere_centre.getVectY();
		double sphere_centre_z = sphere_centre.getVectZ();

		double a = 1; //normalized
		double b = (2*(ray_origin_x-sphere_centre_x)*ray_direction_x) + (2*(ray_origin_y-sphere_centre_y)*ray_direction_y) + (2*(ray_origin_z-sphere_centre_z)*ray_direction_z);
		double c = pow(ray_origin_x-sphere_centre_x, 2) + pow(ray_origin_y-sphere_centre_y, 2) + pow(ray_origin_z-sphere_centre_z, 2) - (radius*radius);
		double discriminant = b*b - 4*c;

		if(discriminant > 0){
			//ray intersects sphere
			double root_1 = ((-1*b-sqrt(discriminant))/2)-0.000001;
			if(root_1>0){
				return root_1; //the smallest positive value
			}else{
				double root_2 = ((sqrt(discriminant)-b)/2)-0.000001;
				return root_2; //or else this is the smallest positive value
			}
		}else{
			//ray missed
			return -1;
		}
	}
};

sphere::sphere(){
	centre = vect(0,0,0);
	radius = 1.0;
	colour = color(0.3,0.2,0.8,0);
}

sphere::sphere(vect centreValue, double radiusValue, color colourValue){
	centre = centreValue;
	radius = radiusValue;
	colour = colourValue;
}

#endif