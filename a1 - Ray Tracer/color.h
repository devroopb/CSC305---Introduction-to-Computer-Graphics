#ifndef _COLOR_H
#define _COLOR_H

class color{
	double red, green, blue, special;

public:
	color();
	color(double,double,double,double);

	double getColorRed(){
		return red;
	}

	double getColorGreen(){
		return green;
	}

	double getColorBlue(){
		return blue;
	}

	double getColorSpecial(){
		return special;
	}

	double setColorRed(double redValue){
		red = redValue;
	}

	double setColorGreen(double greenValue){
		green = greenValue;
	}

	double setColorBlue(double blueValue){
		blue = blueValue;
	}

	double setColorSpecial(double specialValue){
		special = specialValue;
	}

	//manipulates colours for shadows
	double brightness(){
		return (red+green+blue)/3;
	}

	color colorScalar(double scalar){
		return color(red*scalar, green*scalar, blue*scalar, special);
	}

	color colorAdd(color colour){
		return color(red+colour.getColorRed(), green+colour.getColorGreen(), blue+colour.getColorBlue(), special);
	}

	color colorMultiply(color colour){
		return color(red*colour.getColorRed(), green*colour.getColorGreen(), blue*colour.getColorBlue(), special);
	}

	color colorAverage(color colour){
		return color((red+colour.getColorRed())/2, (green+colour.getColorGreen())/2, (blue+colour.getColorBlue())/2, special);
	}

	color clip(){
		double allLight = red + green + blue;
		double excessLight = allLight - 3;
		if(excessLight > 0){
			red = red + excessLight*(red/allLight);
			green = green + excessLight*(green/allLight);
			blue = blue + excessLight*(blue/allLight);
		}
		if(red > 1){
			red = 1;
		}
		if(green > 1){
			green = 1;
		}
		if(blue > 1){
			blue = 1;
		}
		if(red < 0){
			red = 0;
		}
		if(green < 0){
			green = 0;
		}
		if(blue < 0){
			blue = 0;
		}
		return color(red, green, blue, special);
	}
};

color::color(){
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

color::color(double r, double g, double b, double s){
	red = r;
	green = g;
	blue = b;
	special = s;
}

#endif