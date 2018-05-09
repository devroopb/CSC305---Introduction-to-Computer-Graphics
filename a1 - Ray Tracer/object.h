#ifndef _OBJECT_H
#define _OBJECT_H

#include "ray.h"
#include "color.h"
#include "math.h"

class object{
public:
	object();

	virtual color getColor(){
		return color(0.0, 0.0, 0.0, 0);
	}

	virtual vect getNormalAt(vect intersection_position){
		return vect(0, 0, 0);
	}

	virtual double findIntersection(ray r){
			return 0;
	}
};

object::object(){
}

#endif