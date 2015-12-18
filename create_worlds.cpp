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
	auto doorways = doordetection.FindDoorways(workspace_door);

	//Draw PX (point on each site of door) and close the door
	doordetection.DrawPxAndDoors(doorways, workspace_door);

    Tree tree(workspace_door);
    Point start = Point(2391,1300);
    auto door_tree = tree.Tree_generator(start, doorways);

    DOTgraph graph;
    Door door;
    door.adjacent = door_tree;
    door.start.x_pos = 2391;
    door.start.y_pos = 1300;
    graph.visualize(door);
    graph.SaveGraph("graph.dot");

	workspace_door->img->saveImage("doors_detected.pgm");

    std::cout << "Done" << std::endl;

return 0;
}





