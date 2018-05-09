#ifndef _CAMERA_H
#define _CAMERA_H

#include "vect.h"

class camera{
	vect campos, camdir, camright, camdown;

public:
	camera();
	camera(vect, vect, vect, vect);

	vect getCameraPosition(){
		return campos;
	}

	vect getCameraDirection(){
		return camdir;
	}

	vect getCameraRight(){
		return camright;
	}

	vect getCameraDown(){
		return camdown;
	}
};

camera::camera(){
	campos = vect(0,0,0);
	camdir = vect(0,0,1);
	camright = vect(0,0,0);
	camdown = vect(0,0,0);
}

camera::camera(vect pos, vect dir, vect right, vect down){
	campos = pos;
	camdir = dir;
	camright = right;
	camdown = down;

}

#endif