#include "Tree.h"
#include <sstream>
Tree::Tree()
{

}

Tree::Tree(World* world)
{
	this->world = world;
}


std::vector<Door> Tree::door_hitpoint_merge(std::vector<Door> & doorways, std::vector<Point> &wavefront_hitpoint_px){
	std::vector<Door> output;
	bool check1 = false;
	bool check2 = false;

	for (auto px : wavefront_hitpoint_px){
		for(int i = 0; i < doorways.size(); ++i){
			if(doorways[i].px1.visited  || doorways[i].px2.visited ){
				continue;
			}
			if(px.x == doorways[i].px1.x && px.y == doorways[i].px1.y){
				doorways[i].px1.visited = true;
				check1 = true;
				output.push_back(doorways[i]);
			}
			if(px.x == doorways[i].px2.x && px.y == doorways[i].px2.y){
				doorways[i].px2.visited = true;
				check2 = true;
				output.push_back(doorways[i]);
			}
		}
	}
	return output;
}

std::vector<Door> Tree::Tree_generator(Point &start, std::vector<Door> &doorways){

	// find doors from offload station
	auto hit_px_doors = world->Wavefront_DoorScanner(start, 127, 126);

	// Merge doors with hitspoints
	auto doorways_visited = door_hitpoint_merge(doorways, hit_px_doors);


	Point *startPoint;
	std::vector<Door> output;
	for(auto &door: doorways_visited){
		if(door.start.x == 2610){
			std::cout << "point " << door.start.x << "," << door.start.y << " px1: " << door.px1.visited << " px2: " << door.px2.visited << std::endl;
		}
		if(door.px1.visited && door.px2.visited){
			std::cout << "Both visited " << std::endl;
			continue;
		}

		if(door.px1.visited){
			startPoint = &door.px2; // Start from opposite site of door
		} else if(door.px2.visited){
			startPoint = &door.px1;
		} else {
			std::cout << "Room with no doors" << std::endl;
		}

		door.adjacent = Tree_generator(*startPoint, doorways);
		output.push_back(door);
	}
	return output;
}


std::vector<Door> Tree::GenerateNavigationList(Door door){
	std::vector<Door> listOfVectors;
	AddToList(listOfVectors, door);
	return listOfVectors;
}

Door Tree::AddToList(std::vector<Door>& doorList, Door parent){
	for(auto door : parent.adjacent){
		doorList.push_back( AddToList(doorList, door) );
	}
	return parent;
}

