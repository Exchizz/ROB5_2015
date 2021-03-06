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
#include "Robot/Robot.h"
#include "SquareCoverage/SquareCoveragePlanner.h"
#include <list>

int main(){
	Image* rawImg = new Image;
	rawImg->loadImage("complete_map_project.pgm");
	rawImg->maxValue = 8191;
	rawImg->cleanupImage();

	World* Qspace1 = new World(rawImg);
	World* Qspace2 = new World(rawImg);
	World* workspace_door = new World(rawImg);

	World* Workspace_Roomcoverage = new World(rawImg);


	Qspace1->WallExpansion();
	Qspace1->img->saveImage("wallExpansion.pgm");

	Robot robot(Point(2393,1303),Qspace1);

	DoorDetection doordetection;
	auto doorways = doordetection.FindDoorways(workspace_door);
	doordetection.DrawPxAndDoors(doorways, Workspace_Roomcoverage);
	Workspace_Roomcoverage->img->cleanupImage();
	Workspace_Roomcoverage->img->saveImage("saving_closed_doors.pgm");

	std::cout << "number of doors: " << doorways.size() << std::endl;

	//Draw PX (point on each site of door) and close the door
	doordetection.DrawPxAndDoors(doorways, workspace_door);

	Tree tree(workspace_door);
	Point start = Point(2391,1300);
	auto door_tree = tree.Tree_generator(start, doorways);

	DOTgraph graph;
	Door door;
	door.children = door_tree;
	door.start.x = 2432;
	door.start.y = 1330;
	door.FirstPoint = PX1;
	door.px1.x = 2432;
	door.px1.y = 1330;

	graph.visualize(door);
	graph.SaveGraph("graph.dot");

	auto doorsToInspect = tree.GenerateNavigationList(door);

	std::cout << "Vector size: " << doorsToInspect.size() << std::endl;
	int inAll = doorsToInspect.size();

	/*
    Point startPtr;
    int counter = 0;
    //for(auto elm:doorsToInspect ){
    for(int i = 0; i < doorsToInspect.size(); ++i ){
        auto elm = doorsToInspect[i];
        counter++;
        std::cout << "counter: " << counter << std::endl;
        if(elm.start.x == elm.stop.x){
            startPtr.x = elm.start.x;
            startPtr.y = elm.start.y + elm.getLength()/2;
        }else if(elm.start.y == elm.stop.y){
            startPtr.x = elm.start.x + elm.getLength()/2;
            startPtr.y = elm.start.y;
        }else{
            std::cout << "Error non of the above" << std::endl;
        }
        std::cout << startPtr.x << "." << startPtr.y << std::endl;
        robot.goToPoint(startPtr);
    }

	 */
	//    Robot robot2(Point(4519, 76), Qspace1);
	//    robot2.goToPoint(Point(4626, 118));


	workspace_door->img->saveImage("doors_detected.pgm");
	int counter = 0;
	int i = 0;

	std::list<RoomCoverage*> rooms;
    bool clearedRoom = false;
	//    for(auto elm : doorsToInspect){
	for(int i = 0; i < doorsToInspect.size(); i+=2){
		Point Enter = doorsToInspect[i];
		Point Exit = doorsToInspect[i+1];

		RoomCoverage* roomCoverage;
        if(!clearedRoom){
			roomCoverage = new RoomCoverage(Enter, 4, 20, Workspace_Roomcoverage->img);
		}
		Point nextLocation;
		if(Enter.visited && Exit.visited){
			nextLocation = roomCoverage->pathCoverRoom(Enter, Exit);
			robot.goToPoint(nextLocation);
			while(!roomCoverage->roomCleared()){
				std::cout << "moving 1" << std::endl;
				nextLocation = roomCoverage->getNextLocation();
				robot.goToPoint(nextLocation);
			}

            clearedRoom = true;
            delete roomCoverage;
            roomCoverage = rooms.back();
            rooms.pop_back();

		} else {
			nextLocation = roomCoverage->pathGoToDoor(Enter, Exit);
			robot.goToPoint(nextLocation);
			while(!(nextLocation == Exit)){
				std::cout << "moving 2" << std::endl;
				nextLocation = roomCoverage->getNextLocation();
				robot.goToPoint(nextLocation);
			}
            rooms.push_back(roomCoverage);
            clearedRoom = false;
		}

		std::cout << (float)(counter+=2)/inAll*100 << "% done" << "\n" << std::flush;
	}
	robot.savePath("robotPath.pgm");
	std::cout << "distance traveled: " << robot.lengthTraveled() << " pixels: " <<  (float)robot.lengthTraveled()/10 << " meters" << std::endl;

	workspace_door->img->saveImage("doors_detected.pgm");

	std::cout << "Done" << std::endl;

	return 0;
}
