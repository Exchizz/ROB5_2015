/*
 * create_worlds.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */
#include "World/World.h"
#include "Point/Point.h"
#include "DoorDetection/DoorDetection.h"

int main(){
	Image* rawImg = new Image;
	rawImg->loadImage("complete_map_project.pgm");

	World* Qspace1 = new World(rawImg);
	World* Qspace2 = new World(rawImg);
	World* workspace_door = new World(rawImg);

	//Qspace1->img->setPixel(0,0,100);
	Qspace1->WallExpansion();
	std::cout << "Wall expansion i done" << std::endl;
	Qspace2->img->cleanupImage();
	std::cout << "Cleanup i done" << std::endl;

	DoorDetection doordetection;
	auto doorways = doordetection.FindDoorways(8, 5, 50, workspace_door);

	for(auto doorwaysIt : doorways) {
		for(int y = doorwaysIt.start.y_pos; y <= doorwaysIt.stop.y_pos; ++y) {
			workspace_door->img->setPixel(doorwaysIt.start.x_pos, y, 127);
		}
		//std::cout << "There was a door: (" << doorwaysIt.start.x << ", " << doorwaysIt.start.y << ")";
		//std::cout << " ; (" << doorwaysIt.end.x << ", " << doorwaysIt.end.y << ")" << std::endl;
	}

//    Qspace2->img->maxValue = 65535;
//	Qspace2->img->maxValue = 2887;
//	Qspace2->img->cleanupImage();
//	Qspace2->Wavefront_offloading(Point(2391,1300), Point(2875,1300)):
//	Qspace1->img->saveImage("qspace1.pgm");
//	Qspace2->img->saveImage("qspace2.pgm");

//	rawImg->saveImage("origin_image.pgm");
	workspace_door->img->saveImage("doors_detected.pgm");

    //Qspace->wall_expansion();
	//World* Wspace_opendoor = new World(rawImg);
	//Wspace_closeddoor->door_detection();
	//World* Wspace_closeddoor = new World(rawImg);
	//wspace_closeddoor.saveImage("filename.pgm");
    std::cout << "Done" << std::endl;

return 0;
}





