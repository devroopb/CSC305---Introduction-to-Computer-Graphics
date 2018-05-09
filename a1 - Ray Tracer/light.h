#ifndef _LIGHT_H
#define _LIGHT_H
#include "vect.h"
#include "color.h"
#include "source.h"

class light : public source{
	vect position;
	color colour;

public:
	light();
	light(vect,color);

	virtual vect getLightPosition(){
		return position;
	}

	virtual color getLightColor(){
		return colour;
	}
};

light::light(){
	position = vect(0,0,0);
	colour = color(1,1,1,0);
}

light::light(vect p, color c){
	position = p;
	colour = c;
}

#endif