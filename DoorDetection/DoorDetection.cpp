#include "DoorDetection.h"


DoorDetection::DoorDetection(){}

std::vector<Door>  DoorDetection::FindDoorways(const World* world){
	std::vector<Door> doorways;
	World* doorWorld_ptr = new World(world->img);

	FindVerticalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX / 3, DOORWIDTH_MIN, DOORWIDTH_MAX / 3, doorWorld_ptr, doorways);
	drawDoors(doorWorld_ptr, doorways);
	FindHorizontalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX, DOORWIDTH_MIN, DOORWIDTH_MAX -2, doorWorld_ptr, doorways);
	drawDoors(doorWorld_ptr, doorways);
	FindVerticalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX, DOORWIDTH_MIN, DOORWIDTH_MAX, doorWorld_ptr, doorways);

	//for(auto doorwaysIt : doorways) {
	for(unsigned int i = 0; i < doorways.size();++i){

		//Door vertical
		if(doorways[i].start.x == doorways[i].stop.x){
			doorways[i].px1.y = doorways[i].start.y + (doorways[i].getLength())/2;
			doorways[i].px1.x = doorways[i].start.x - 1 ;

			doorways[i].px2.y = doorways[i].start.y + (doorways[i].getLength())/2;
			doorways[i].px2.x = doorways[i].start.x + 1 ;
		} // Door horizontal
		else {
			doorways[i].px1.y = doorways[i].start.y - 1;
			doorways[i].px1.x = doorways[i].start.x + (doorways[i].getLength())/2 ;

			doorways[i].px2.y = doorways[i].start.y + 1;
			doorways[i].px2.x = doorways[i].start.x + (doorways[i].getLength())/2 ;
		}

	}

	return doorways;
}


void DoorDetection::DrawPxAndDoors(std::vector<Door>& doorways, World* world){
	for(unsigned int i = 0; i < doorways.size();++i){
		// Mark dooropening (vertical)
		for(int y = doorways[i].start.y; y <= doorways[i].stop.y; ++y) {
			world->img->setPixel(doorways[i].start.x, y, 127);
		}
		// Mark dooropenings (horizantal)
		for(int x = doorways[i].start.x; x <= doorways[i].stop.x; ++x) {
			world->img->setPixel(x, doorways[i].start.y, 127);
		}

		world->img->setPixel(doorways[i].px1.x,  doorways[i].px1.y, 126);
		world->img->setPixel(doorways[i].px2.x,  doorways[i].px2.y, 126);
	}
}

void DoorDetection::drawDoors(World* world, std::vector<Door> &doorways)
{
	for(auto doorwaysIt : doorways) {
		for(int x = doorwaysIt.start.x; x <= doorwaysIt.stop.x; ++x) {
			for(int y = doorwaysIt.start.y; y <= doorwaysIt.stop.y; ++y) {
				world->img->setPixel(x, y, 0);
			}
		}
	}
}

void DoorDetection::FindHorizontalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways){
	std::vector<Door> newDoors, lastDoors;
	std::vector<Door>::iterator newDoorsIt, lastDoorsIt;

	Door aDoor;
	bool noLine = true;

	for(int y = 0; y < world->img->getHeight(); y++) {
		for(int x = 0; x < world->img->getWidth(); x++) {
			//find line segments
			if(world->img->getPixel(x, y) == 0 || (x == world->img->getWidth() - 1)) {
				if(noLine == false) {
					//end the last line segment
					aDoor.stop.y = y;
					if(x == world->img->getWidth() - 1)
						aDoor.stop.x = x;
					else
						aDoor.stop.x = x - 1;
					//and save it
					newDoors.push_back(aDoor);
					noLine = true;
				}
			} else {
				if(noLine == true) {
					noLine = false;
					//start new line segment
					aDoor.start.x = x;
					aDoor.start.y = y;
				}
			}
		}
		//Compare line segments in queue.
		newDoorsIt = newDoors.begin();
		lastDoorsIt = lastDoors.begin();

		while(lastDoorsIt != lastDoors.end() && newDoorsIt != newDoors.end()) {
			//It is a door?
			if(lastDoorsIt->getLength() < newDoorsIt->getLength()) {
				//is it to long to be a door?
				if(!(lastDoorsIt->getLength() > doorWidth_max) && !(lastDoorsIt->getLength() < doorWidth_min)) {
					//Is it on the edges of the map?
					if(lastDoorsIt->start.x != 0 && lastDoorsIt->stop.x != (world->img->getWidth() - 1)) {
						//Does the space behind become bigger and is it not to long?
						int stepback = 0;
						int overStartPixelValue, underEndPixelValue;
						bool keepLooping = true;

						while(stepback < doorThickness_max && keepLooping == true) {
							stepback++;

							for(int i = lastDoorsIt->start.x; i <= lastDoorsIt->stop.x; i++){
								if(world->img->getPixel(i, lastDoorsIt->start.y - stepback) == 0) {
									keepLooping = false;
									break;
								}
							}

							overStartPixelValue = world->img->getPixel(lastDoorsIt->start.x - 1, lastDoorsIt->start.y - stepback);
							underEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x + 1, lastDoorsIt->stop.y - stepback);

							if(overStartPixelValue != 0 || underEndPixelValue != 0) {
								//it is getting larger (door)
								if(stepback >= doorThickness_min)
									doorways.push_back(*lastDoorsIt);
								keepLooping = false;
							}
						}
					}
				}
			}

			//Are the line segments adjacent to each other?
			do {
				//std::cout << "I am looping" << std::endl;
				if(lastDoorsIt->stop.x == newDoorsIt->stop.x) {
					newDoorsIt++;
					lastDoorsIt++;
				}
				else if(lastDoorsIt->stop.x > newDoorsIt->stop.x) {
					newDoorsIt++;
				}
				else {
					lastDoorsIt++;
				}

				if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
					break;

				if(lastDoorsIt->start.x >= newDoorsIt->stop.x) {
					newDoorsIt++;
				}
				else if(lastDoorsIt->stop.x <= newDoorsIt->start.x) {
					lastDoorsIt++;
				}
			} while(lastDoorsIt->stop.x < newDoorsIt->start.x || lastDoorsIt->start.x > newDoorsIt->stop.x);

			if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
				continue;
		}

		//Move new line segments to last line segments.
		lastDoors = newDoors;
		newDoors.clear();
	}
}

void DoorDetection::FindVerticalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways){
	std::vector<Door> newDoors, lastDoors;
	std::vector<Door>::iterator newDoorsIt, lastDoorsIt;

	Door aDoor;
	bool noLine = true;

	for(int x = 0; x < world->img->getWidth(); x++) {
		for(int y = 0; y < world->img->getHeight(); y++) {
			//find line segments
			if(world->img->getPixel(x, y) == 0 || (y == world->img->getHeight() - 1)) {
				if(noLine == false) {
					//end the last line segment
					aDoor.stop.x = x;
					if(y == world->img->getHeight() - 1)
						aDoor.stop.y = y;
					else
						aDoor.stop.y = y - 1;
					//and save it
					newDoors.push_back(aDoor);
					noLine = true;
				}
			} else {
				if(noLine == true) {
					noLine = false;
					//start new line segment
					aDoor.start.x = x;
					aDoor.start.y = y;
				}
			}
		}
		//Compare line segments in queue.
		newDoorsIt = newDoors.begin();
		lastDoorsIt = lastDoors.begin();

		while(lastDoorsIt != lastDoors.end() && newDoorsIt != newDoors.end()) {
			//It is a door?
			if(lastDoorsIt->getLength() < newDoorsIt->getLength()) {
				//is it to long to be a door?
				if(!(lastDoorsIt->getLength() > doorWidth_max) && !(lastDoorsIt->getLength() < doorWidth_min)) {
					//Is it on the edges of the map?
					if(lastDoorsIt->start.y != 0 && lastDoorsIt->stop.y != (world->img->getHeight() - 1)) {
						//Does the space behind become bigger and is it not to long?
						int stepback = 0;
						int overStartPixelValue, underEndPixelValue;
						bool keepLooping = true;

						while(stepback < doorThickness_max && keepLooping == true) {
							stepback++;

							for(int i = lastDoorsIt->start.y; i <= lastDoorsIt->stop.y; i++){
								if(world->img->getPixel(lastDoorsIt->start.x - stepback, i) == 0) {
									keepLooping = false;
									break;
								}
							}

							overStartPixelValue = world->img->getPixel(lastDoorsIt->start.x - stepback, lastDoorsIt->start.y - 1);
							underEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x - stepback, lastDoorsIt->stop.y + 1);

							if(overStartPixelValue != 0 || underEndPixelValue != 0) {
								//it is getting larger (door)
								doorways.push_back(*lastDoorsIt);
								keepLooping = false;
							}
						}
					}
				}
			}

			//Are the line segments adjacent to each other?
			do {
				if(lastDoorsIt->stop.y == newDoorsIt->stop.y) {
					newDoorsIt++;
					lastDoorsIt++;
				}
				else if(lastDoorsIt->stop.y > newDoorsIt->stop.y) {
					newDoorsIt++;
				}
				else {
					lastDoorsIt++;
				}

				if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
					break;

				if(lastDoorsIt->start.y >= newDoorsIt->stop.y) {
					newDoorsIt++;
				}
				else if(lastDoorsIt->stop.y <= newDoorsIt->start.y) {
					lastDoorsIt++;
				}
			} while(lastDoorsIt->stop.y < newDoorsIt->start.y || lastDoorsIt->start.y > newDoorsIt->stop.y);

			if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
				continue;
		}

		//Move new line segments to last line segments.
		lastDoors = newDoors;
		newDoors.clear();
	}
}
