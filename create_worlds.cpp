/*
 * create_worlds.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */
#include "World/World.h"
#include "Point.h"

int main(){
	Image* rawImg = new Image;
	rawImg->loadImage("complete_map_project.pgm");

	World* Qspace1 = new World(rawImg);
	World* Qspace2 = new World(rawImg);

	//Qspace1->img->setPixel(0,0,100);
	Qspace1->WallExpansion();
	Qspace2->img->setPixel(0,0,150);

    Qspace2->img->maxValue = 65535;
    Qspace2->Wavefront(Point(2391,1300));

    Qspace1->img->saveImage("qspace1.pgm");

    Qspace2->img->saveImage("qspace2.pgm");

    rawImg->saveImage("origin_image.pgm");

    //Qspace->wall_expansion();
	//World* Wspace_opendoor = new World(rawImg);
	//Wspace_closeddoor->door_detection();
	//World* Wspace_closeddoor = new World(rawImg);
	//wspace_closeddoor.saveImage("filename.pgm");


return 0;
}





