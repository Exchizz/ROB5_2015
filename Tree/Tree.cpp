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


	for (auto px : wavefront_hitpoint_px){
		for(int i = 0; i < doorways.size(); ++i){
			if(px == doorways[i].px1 && !doorways[i].px1.visited){
				doorways[i].px1.visited = true;
				output.push_back(doorways[i]);
			}
			if(px == doorways[i].px2 && !doorways[i].px2.visited){
				doorways[i].px2.visited = true;
				output.push_back(doorways[i]);
			}
		}
	}

	return output;
}

std::vector<Door> Tree::Tree_generator(Point &start, std::vector<Door> &doorways){

	//Loop through all doors and check if already scanned. If not continue and scan, else return empty vector.
	for(auto elm : doorways){
		// Look for px1
		if(elm.px1 == start){
			if(elm.px1.visited){
				std::vector<Door> empty;
				return empty;
			} else {
				break;
			}
		}
		// Look for px2
		if(elm.px2 == start){
			if(elm.px2.visited){
				std::vector<Door> empty;
				return empty;
			} else {
				break;
			}
		}
	}


	auto hit_px_doors = world->Wavefront_DoorScanner(start, 127, 126);

	// Merge doors with hitspoints
	auto doorways_visited = door_hitpoint_merge(doorways, hit_px_doors);


	Point *startPoint;
	std::vector<Door> output;
	for(auto &door: doorways_visited){

		if(door.px1.visited && door.px2.visited){
			continue;
		}

		if(door.px1.visited){
			startPoint = &door.px2; // Start from opposite site of door
			door.FirstPoint = PX2;
		} else if(door.px2.visited){
			startPoint = &door.px1;
			door.FirstPoint = PX1;
		} else {
			std::cout << "Room with no doors" << std::endl;
		}


		door.children = Tree_generator(*startPoint, doorways);
		output.push_back(door);
	}
	return output;
}



std::vector<Point> Tree::GenerateNavigationList(Door door){
	std::vector<Point> plannedPath;
	AddToList(plannedPath, door);
	return plannedPath;
}

Door Tree::AddToList(std::vector<Point> &plannedPath, Door &parent){
	if(parent.children.size() == 0){
		//if it is a leaf, turn around..
		if(parent.FirstPoint == 1) {
			parent.px1.visited = true;
			plannedPath.push_back(parent.px1);
			plannedPath.push_back(parent.px1);
			parent.px1.visited = false;
		}
		else if(parent.FirstPoint == 2) {
			parent.px2.visited = true;
			plannedPath.push_back(parent.px2);
			plannedPath.push_back(parent.px2);
			parent.px2.visited = false;
		}
	} else {
		int i = 0;
		for(i = 0; i < parent.children.size(); ++i){
		//going down tree branches
			//push parent
			if(parent.FirstPoint == 1) {
				parent.px1.visited = false;
				plannedPath.push_back(parent.px1);
			}
			else if(parent.FirstPoint == 2) {
				parent.px2.visited = false;
				plannedPath.push_back(parent.px2);
			}
			//push child
			if(parent.children[i].FirstPoint == 1) {
				parent.children[i].px2.visited = false;
				plannedPath.push_back(parent.children[i].px2);
			}
			else if(parent.children[i].FirstPoint == 2) {
				parent.children[i].px1.visited = false;
				plannedPath.push_back(parent.children[i].px1);
			}

			//intermediate steps
			if(parent.children[i].FirstPoint == 1) {
				parent.px2.visited = false;
				plannedPath.push_back(AddToList(plannedPath, parent.children[i]).px2);
			}
			else if(parent.children[i].FirstPoint == 2) {
				parent.px1.visited = false;
				plannedPath.push_back(AddToList(plannedPath, parent.children[i]).px1);
			}

			if(parent.children[i].cover){
				parent.cover = 1;
				parent.px1.visited = true;
				parent.px2.visited = true;
			}
			//going up tree branches

			if(parent.FirstPoint == 1) {
				parent.px1.visited = false;		//husk her..
				plannedPath.push_back(parent.px1);
				parent.px1.visited = false;
			}
			else if(parent.FirstPoint == 2) {
				parent.px2.visited = false;		//husk her..
				plannedPath.push_back(parent.px2);
				parent.px2.visited = false;
			}
			parent.px1.visited = 0;
			parent.px2.visited = 0;
			parent.cover = 0;
		}
		parent.cover = 1;
	}
	return parent;
}


/*std::vector<Door> Tree::GenerateNavigationList(Door door){
	std::vector<Door> listOfVectors;
	AddToList(listOfVectors, door);
	return listOfVectors;
}

Door Tree::AddToList(std::vector<Door> &doorList, Door &parent){


		if(parent.children.size() == 0){
	//if it is a leaf, turn around..
			parent.cover = 1;
			doorList.push_back(parent);
			doorList.push_back(parent);
			parent.cover = 0;
		} else {
			int i = 0;
			for(i = 0; i < parent.children.size(); ++i){
			//going down tree branches
				doorList.push_back(parent);
				doorList.push_back(parent.children[i]);
			//intermediate steps
				doorList.push_back( AddToList(doorList, parent.children[i]) );
				if(parent.children[i].cover){
					parent.cover = 1;
				}
			//going up tree branches
				doorList.push_back(parent);
				parent.cover = 0;
			}
			parent.cover = 1;
		}
	return parent;
}*/
