/*
 * create_worlds.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */
#include "World/World.h"
#include "Point/Point.h"
#include "DoorDetection/DoorDetection.h"
#include "Tree/Tree.h"
#include "DOTgraph/DOTgraph.h"
int main(){
	Image* rawImg = new Image;
	rawImg->loadImage("complete_map_project.pgm");
    rawImg->maxValue = 2887;
    rawImg->cleanupImage();

	World* Qspace1 = new World(rawImg);
	World* Qspace2 = new World(rawImg);
	World* workspace_door = new World(rawImg);

    Qspace1->WallExpansion();

	DoorDetection doordetection;
	auto doorways = doordetection.FindDoorways(8, 5, 50, workspace_door);

    //for(auto doorwaysIt : doorways) {
    for(unsigned int i = 0; i < doorways.size();++i){

        //Door vertical
        if(doorways[i].start.x_pos == doorways[i].stop.x_pos){
            doorways[i].px1.y_pos = doorways[i].start.y_pos + (doorways[i].stop.y_pos - doorways[i].start.y_pos)/2;
            doorways[i].px1.x_pos = doorways[i].start.x_pos - 1 ;

            doorways[i].px2.y_pos = doorways[i].start.y_pos + (doorways[i].stop.y_pos - doorways[i].start.y_pos)/2;
            doorways[i].px2.x_pos = doorways[i].start.x_pos + 1 ;
        }

        // Mark dooropening
        for(int y = doorways[i].start.y_pos; y <= doorways[i].stop.y_pos; ++y) {
            workspace_door->img->setPixel(doorways[i].start.x_pos, y, 127);
        }

        // Mark left and right side of doorway (px1, px2)
        workspace_door->img->setPixel(doorways[i].px1.x_pos,  doorways[i].px1.y_pos, 126);
        workspace_door->img->setPixel(doorways[i].px2.x_pos,  doorways[i].px2.y_pos, 126);

    }


    Tree tree(workspace_door);
    Point start = Point(2391,1300);
    auto door_tree = tree.Tree_generator(start, doorways);


    for(auto elm: door_tree){
    	std::cout << "Door L1: " << elm.start.x_pos << "," << elm.start.y_pos << std::endl;
    	for(auto subelm : elm.adjacent){
        	std::cout << "\t Door L2: " << subelm.start.x_pos << "," << subelm.start.y_pos << std::endl;
    		for(auto subsubelm : subelm.adjacent){
            	std::cout << "\t \t Door L3: " << subsubelm.start.x_pos << "," << subsubelm.start.y_pos << std::endl;
    		}
    	}
    }

    DOTgraph graph;
    Door door;
    door.adjacent = door_tree;
    door.start.x_pos = 2391;
    door.start.y_pos = 1300;
    graph.visualize(door);
    graph.SaveGraph("graph.dot");


    /*
    // find doors from offload station
    auto hit_px_doors = workspace_door->Wavefront_DoorScanner(Point(2391,1300), 127,126);

    // Merge doors with hitspoints
    auto doorways_visisted = tree.door_hitpoint_merge(doorways, hit_px_doors);

    //Run through every door and mark hitpoint
    for (auto hitpoint : doorways_visisted){
        if(hitpoint.px1_visited){
            workspace_door->img->setPixel(hitpoint.px1.x_pos, hitpoint.px1.y_pos, 2887);
        }

        if(hitpoint.px2_visited){
            workspace_door->img->setPixel(hitpoint.px2.x_pos, hitpoint.px2.y_pos, 2887);
        }
    }
    */

	workspace_door->img->saveImage("doors_detected.pgm");

    std::cout << "Done" << std::endl;

return 0;
}





