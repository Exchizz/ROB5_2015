#include "Tree.h"

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

	for(int i = 0; i < doorways.size(); ++i){
		for (auto px : wavefront_hitpoint_px){
			//std::cout << "wavefront hitpoints: " << px.x_pos << ", " << px.y_pos << std::endl;
			if(px.x_pos == doorways[i].px1.x_pos && px.y_pos == doorways[i].px1.y_pos){
				doorways[i].px1.visited = true;
				check1 = true;
			}
			if(px.x_pos == doorways[i].px2.x_pos && px.y_pos == doorways[i].px2.y_pos){
				doorways[i].px2.visited = true;
				check2 = true;
			}
		}

		if(check1 || check2){
			check1 = false;
			check2 = false;
			output.push_back(doorways[i]);
		}
	}

	return output;
}

std::vector<Door> Tree::Tree_generator(Point &start, std::vector<Door> &doorways){

	static int counter = 50;

	if(--counter <= 0){
		std::vector<Door> rtn;
		return rtn;
	}
	// find doors from offload station
	auto hit_px_doors = world->Wavefront_DoorScanner(start, 127, 126);

	// Merge doors with hitspoints
	auto doorways_visited = door_hitpoint_merge(doorways, hit_px_doors);

    for(auto elm: doorways_visited){
    	if(elm.start.x_pos == 2610){
    		std::cout << "doors visisted: " << elm.start.x_pos << "," << elm.start.y_pos << " px1: " << elm.px1.visited << " px2: " << elm.px2.visited << std::endl;
    	}
    }

	Point *startPoint;
	std::vector<Door> output;
	for(auto &door: doorways_visited){
		if(door.start.x_pos == 2610){
			std::cout << "point " << door.start.x_pos << "," << door.start.y_pos << " px1: " << door.px1.visited << " px2: " << door.px2.visited << std::endl;
		}
		if(door.px1.visited && door.px2.visited){
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
