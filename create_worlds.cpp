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
    std::cout << "Wall expansion i done" << std::endl;
    Qspace2->img->cleanupImage();
    std::cout << "Cleanup i done" << std::endl;

//    Qspace2->img->maxValue = 65535;
    Qspace2->img->maxValue = 2887;
    Qspace2->img->cleanupImage();
    Qspace2->Wavefront_offloading(Point(2391,1300), Point(2875,1300));

    Qspace1->img->saveImage("qspace1.pgm");

    Qspace2->img->saveImage("qspace2.pgm");

    rawImg->saveImage("origin_image.pgm");

    //Qspace->wall_expansion();
	//World* Wspace_opendoor = new World(rawImg);
	//Wspace_closeddoor->door_detection();
	//World* Wspace_closeddoor = new World(rawImg);
	//wspace_closeddoor.saveImage("filename.pgm");
    std::cout << "Done" << std::endl;

return 0;
}





