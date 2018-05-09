/*
  Devroop Banerjee
  V00837868
  CSC 305 - Assignment 1
  Reference:- https://www.youtube.com/watch?v=k_aRiYSXcyo&list=PLHm_I0tE5kKPPWXkTTtOn8fkcwEGZNETh
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "vect.h"
#include "ray.h"
#include "camera.h"
#include "color.h"
#include "light.h"
#include "sphere.h"
#include "object.h"
#include "plane.h"
#include "source.h"

using namespace std;

struct RGBType{
	double r, g, b;
};



void savebmp(const char *filename, int w, int h, int dpi, RGBType *data){
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54+s;
	double factor = 39.375;
	int m = static_cast<int>(factor);
	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize>>8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w>>8);
	bmpinfoheader[6] = (unsigned char)(w>>16);
	bmpinfoheader[7] = (unsigned char)(w>>24);

	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);

	bmpinfoheader[29] = (unsigned char)(s);
	bmpinfoheader[30] = (unsigned char)(s>>8);
	bmpinfoheader[31] = (unsigned char)(s>>16);
	bmpinfoheader[32] = (unsigned char)(s>>24);

	f = fopen(filename, "wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for(int i = 0; i < k; i++){
		RGBType rgb = data[i];
		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;
		unsigned char color[3] = {(int)floor (blue),(int)floor (green),(int)floor (red)};
		fwrite(color, 1, 3, f);
	}
	fclose(f);
}



int winningObjectIndex(vector<double> object_intersections){
	//index of intersections
	int index_of_minimum_value;
	
	if(object_intersections.size() == 0){ //no intersections
		return -1;
	}else if(object_intersections.size() == 1){ //only 1 intersection
		if(object_intersections.at(0) > 0){ //index of minimum value
			return 0;
		}else{ //intersection value is negative
			return -1;
		}
	}else{ //more than 1 intersection
		//need to find object closest to camera to find colour of pixel
		double max = 0;
		
		for(int i = 0; i < object_intersections.size(); i++){
			if(max < object_intersections.at(i)){
				max = object_intersections.at(i);
			}
		}
		
		//start from max to find min positive value
		if(max > 0){
			for(int index = 0; index < object_intersections.size(); index++){
				if(object_intersections.at(index) > 0 && object_intersections.at(index) <= max){
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}
			return index_of_minimum_value;
		}else{
			return -1; //no intersections
		}
	}
}



color getColorAt(vect intersection_position, vect intersecting_ray_direction, vector<object*> scene_objects, int index_of_winning_object, vector<source*> light_sources, double accuracy, double ambientLight){
	color winning_object_color = scene_objects.at(index_of_winning_object)->getColor();
	vect winning_object_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);

	//floor pattern
	if(winning_object_color.getColorSpecial() == 2){
		int square = (int)floor(intersection_position.getVectX()) + (int)floor(intersection_position.getVectZ());

		//black tile
		if((square % 2) == 0){
			winning_object_color.setColorRed(0);
			winning_object_color.setColorGreen(0);
			winning_object_color.setColorBlue(0);
		}else{
		//white  tile
			winning_object_color.setColorRed(1);
			winning_object_color.setColorGreen(1);
			winning_object_color.setColorBlue(1);
		}
	}

	color final_color = winning_object_color.colorScalar(ambientLight); //scale colour by ambient light

	//for reflection
	if(winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
		//reflection from object with specular value
		double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
		vect scalar1 = winning_object_normal.vectMult(dot1);
		vect add1 = scalar1.vectAdd(intersecting_ray_direction);
		vect scalar2 = add1.vectMult(2);
		vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
		vect reflection_direction = add2.normalize();
		ray reflection_ray(intersection_position, reflection_direction);

		//determine what ray intersects with
		vector<double> reflection_intersections;

		for(int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++){
			reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
		}

		int index_of_winning_object_with_reflection = winningObjectIndex(reflection_intersections);
		if(index_of_winning_object_with_reflection != -1){
			
			//reflection ray missed
			if(reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy){
				
				//determine position,direction at point of intersection with reflection ray
				vect reflection_intersection_position = intersection_position.vectAdd(reflection_direction.vectMult(reflection_intersections.at(index_of_winning_object_with_reflection)));
				vect reflection_intersection_ray_direction = reflection_direction;

				//recursive call to getColorAt so that rays can keep refelecting off multiple surfaces
				color reflection_intersection_color = getColorAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, index_of_winning_object_with_reflection, light_sources, accuracy, ambientLight);
				final_color = final_color.colorAdd(reflection_intersection_color.colorScalar(winning_object_color.getColorSpecial()));
			}
		}
	}

	for(int light_index = 0; light_index < light_sources.size(); light_index++){
		vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
		float cos = winning_object_normal.dotProduct(light_direction);

		//shadows test
		if(cos > 0){
			bool shadowed = false;
			vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize();
			float distance_to_light_magnitude = distance_to_light.magnitude();
			ray shadow_ray(intersection_position, light_sources.at(light_index)->getLightPosition().vectAdd(intersection_position.negative()).normalize());
			vector<double> secondary_intersections;

			for(int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++){
				secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
			}

			//loop through secondary intersection and if value less than distance to light, then shadow
			for(int c = 0; c < secondary_intersections.size(); c++){
				if(secondary_intersections.at(c) > accuracy){
					if(secondary_intersections.at(c) <= distance_to_light_magnitude){
						shadowed = true;
					}
					break;
				}
			}

			if(shadowed == false){
				final_color = final_color.colorAdd(winning_object_color.colorMultiply(light_sources.at(light_index)->getLightColor()).colorScalar(cos));
				
				if(winning_object_color.getColorSpecial() > 0 && winning_object_color.getColorSpecial() <= 1){
					
					//for special
					double dot1 = winning_object_normal.dotProduct(intersecting_ray_direction.negative());
					vect scalar1 = winning_object_normal.vectMult(dot1);
					vect add1 = scalar1.vectAdd(intersecting_ray_direction);
					vect scalar2 = add1.vectMult(2);
					vect add2 = intersecting_ray_direction.negative().vectAdd(scalar2);
					vect reflection_direction = add2.normalize();
					double specular = reflection_direction.dotProduct(light_direction);
					
					if(specular > 0){
						specular = pow(specular, 10);
						final_color = final_color.colorAdd(light_sources.at(light_index)->getLightColor().colorScalar(specular*winning_object_color.getColorSpecial()));
					}
				}
			}
		}
	}
	return final_color.clip();
}

int thisone, AA_index;



int main(int argc, char *argv[]){
	cout << "rendering..." << endl;

	int dpi = 72;
	int width = 1920;
	int height = 1080;
	int n = width*height;
	int AAdepth = 5; //takes so much time mostly because of this and resolution

	RGBType *pixels = new RGBType [n];
	double AAthreshold = 0.1;
	double aspectRatio = (double)width/(double)height;
	double ambientLight = 0.2;
	double accuracy = 0.000001;

	vect A(2.5,-0.5,1.5);
	vect B(-2.5,0.35,-2.5);
	vect O (0,0,0);
	vect X (1,0,0);
	vect Y (0,1,0);
	vect Z (0,0,1);

	//controls the perspective, colours, light source
	vect look_at(0,0,0);
	vect campos(6,2,-4);
	vect diff_btw(campos.getVectX()-look_at.getVectX(), campos.getVectY()-look_at.getVectY(), campos.getVectZ()-look_at.getVectZ());
	vect camdir = diff_btw.negative().normalize();
	vect camright = Y.crossProduct(camdir).normalize();
	vect camdown = camright.crossProduct(camdir);
	
	//colours
	camera scene_cam(campos,camdir,camright,camdown);
	color white_light(1, 1, 1, 0);
	color really_green(0.13, 0.55, 0.13, 0.5);
	color maroon(0.5, 0.25, 0.25, 2);
	color black(0, 0, 0, 0);
	color cyan(0, 1, 1, 0.3);
	color very_red(0.55, 0, 0, 0.7);
	
	vect light_position(-5, 7, -6);
	light scene_light(light_position, white_light);
	vector<source*> light_sources;
	light_sources.push_back(dynamic_cast<source*>(&scene_light));
	
	//objects in scene
	sphere scene_sphere(O, 1, really_green);
	sphere scene_sphere2(A, 0.5, cyan);
	sphere scene_sphere3(B, 1.4, very_red);
	plane scene_plane(Y, -1, maroon);

	//push objects into vector
	vector<object*> scene_objects;
	scene_objects.push_back(dynamic_cast<object*>(&scene_sphere));
	scene_objects.push_back(dynamic_cast<object*>(&scene_sphere2));
	scene_objects.push_back(dynamic_cast<object*>(&scene_sphere3));
	scene_objects.push_back(dynamic_cast<object*>(&scene_plane));

	double xamnt, yamnt, tempRed, tempGreen, tempBlue;

	for(int x = 0; x < width; x++){
		for(int y = 0; y < height; y++){
			//return colour 
			thisone = y*width + x;

			//empty pixels
			double tempRed[AAdepth*AAdepth], tempGreen[AAdepth*AAdepth], tempBlue[AAdepth*AAdepth];

			for(int AAx = 0; AAx < AAdepth; AAx++){
				for(int AAy = 0; AAy < AAdepth; AAy++){
					AA_index = AAy*AAdepth + AAx;
					srand(time(0));

					//ray from camera to pixels
					if(AAdepth == 1){
						//no anti-aliasing
						if(width > height){
							xamnt = ((x+0.5)/width)*aspectRatio - (((width-height)/(double)height)/2);
							yamnt = ((height-y)+0.5)/height;
						}else if(height > width){
							xamnt = (x+0.5)/width;
							yamnt = (((height-y)+0.5)/height)/aspectRatio - (((height-width)/(double)width)/2);
						}else{
							xamnt = (x+0.5)/width;
							yamnt = ((height-y)+0.5)/height;
						}
					}else{
						//anti-aliasing
						if(width > height){
							xamnt = ((x+(double)AAx/((double)AAdepth-1))/width)*aspectRatio - (((width-height)/(double)height)/2);
							yamnt = ((height-y)+(double)AAx/((double)AAdepth-1))/height;
						}else if(height > width){
							xamnt = (x+(double)AAx/((double)AAdepth-1))/width;
							yamnt = (((height-y)+(double)AAx/((double)AAdepth-1))/height)/aspectRatio - (((height-width)/(double)width)/2);
						}else{
							xamnt = (x+(double)AAx/((double)AAdepth-1))/width;
							yamnt = ((height-y)+(double)AAx/((double)AAdepth-1))/height;
						}
					}

					vect cam_ray_origin = scene_cam.getCameraPosition();
					vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt-0.5).vectAdd(camdown.vectMult(yamnt-0.5))).normalize();
					ray cam_ray(cam_ray_origin, cam_ray_direction);
					vector<double> intersections;

					//check if it intersects
					for(int index = 0; index < scene_objects.size(); index++){
						intersections.push_back(scene_objects.at(index)->findIntersection(cam_ray));
					}

					//object closest to camera; want its colour
					int index_of_winning_object = winningObjectIndex(intersections);

					if(index_of_winning_object == -1){
						//set black background
						tempRed[AA_index] = 0;
						tempGreen[AA_index] = 0;
						tempBlue[AA_index] = 0;
					}else{
						//index of object in scene
						if(intersections.at(index_of_winning_object) > accuracy){
							//position, direction vectors at intersection
							vect intersection_position = cam_ray_origin.vectAdd(cam_ray_direction.vectMult(intersections.at(index_of_winning_object)));
							vect intersecting_ray_direction = cam_ray_direction;
							color intersection_color = getColorAt(intersection_position, intersecting_ray_direction, scene_objects, index_of_winning_object, light_sources, accuracy, ambientLight);
							tempRed[AA_index] = intersection_color.getColorRed();
							tempGreen[AA_index] = intersection_color.getColorGreen();
							tempBlue[AA_index] = intersection_color.getColorBlue();
						}
					}
				}
			}

			//average pixel colour
			double totalRed = 0;
			double totalGreen = 0;
			double totalBlue = 0;

			for(int iRed = 0; iRed < AAdepth*AAdepth; iRed++){
				totalRed = totalRed + tempRed[iRed];
			}

			for(int iGreen = 0; iGreen < AAdepth*AAdepth; iGreen++){
				totalGreen = totalGreen + tempGreen[iGreen];
			}

			for(int iBlue = 0; iBlue < AAdepth*AAdepth; iBlue++){
				totalBlue = totalBlue + tempBlue[iBlue];
			}

			double avgRed = totalRed/(AAdepth*AAdepth);
			double avgGreen = totalGreen/(AAdepth*AAdepth);
			double avgBlue = totalBlue/(AAdepth*AAdepth);

			pixels[thisone].r = avgRed;
			pixels[thisone].g = avgGreen;
			pixels[thisone].b = avgBlue;
		}
	}

	savebmp("scene.bmp", width, height, dpi, pixels);
	delete pixels, tempRed, tempGreen, tempBlue;
	return 0;
}