#include "Image/Image.h"


int main(){

	Image image;

	image.loadImage("complete_map_project.pgm");
	image.saveImage("outfrom_test.pgm");


return 0;
}

