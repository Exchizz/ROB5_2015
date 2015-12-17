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
    rawImg->maxValue = 2887;
    rawImg->cleanupImage();



	World* Qspace1 = new World(rawImg);
	World* Qspace2 = new World(rawImg);
	World* workspace_door = new World(rawImg);
    std::cout << "changed on world: " << workspace_door->img->maxValue  << std::endl;

	//Qspace1->img->setPixel(0,0,100);
	Qspace1->WallExpansion();
    //std::cout << "Wall expansion i done" << std::endl;
    //Qspace2->img->cleanupImage();
    //std::cout << "Cleanup i done" << std::endl;

	DoorDetection doordetection;
	auto doorways = doordetection.FindDoorways(8, 5, 50, workspace_door);

    //for(auto doorwaysIt : doorways) {
    for(unsigned int i = 0; i < doorways.size();++i){
        //Door vertical
        if(doorways[i].start.x_pos == doorways[i].stop.x_pos){
            doorways[i].px1.y_pos= doorways[i].start.y_pos;
            doorways[i].px1.x_pos = doorways[i].start.x_pos - 1 ;

            doorways[i].px2.y_pos= doorways[i].start.y_pos;
            doorways[i].px2.x_pos = doorways[i].start.x_pos + 1 ;
        } else { //Door horizontal
            doorways[i].px1.x_pos= doorways[i].start.x_pos;
            doorways[i].px1.y_pos = doorways[i].start.y_pos - 1 ;

            doorways[i].px2.x_pos= doorways[i].start.x_pos;
            doorways[i].px2.y_pos = doorways[i].start.y_pos + 1 ;
        }
        for(int y = doorways[i].start.y_pos; y <= doorways[i].stop.y_pos; ++y) {
            workspace_door->img->setPixel(doorways[i].start.x_pos, y, 127);
        }

//      workspace_door->img->setPixel(doorways[i].start.x_pos,  doorways[i].start.y_pos+(doorways[i].getLength()/2), 127);
        workspace_door->img->setPixel(doorways[i].px1.x_pos,  doorways[i].px1.y_pos+(doorways[i].getLength()/2), 126);
        workspace_door->img->setPixel(doorways[i].px2.x_pos,  doorways[i].px2.y_pos+(doorways[i].getLength()/2), 126);

    }

    workspace_door->Wavefront_treecreation(Point(2391,1300), 127,126);
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





