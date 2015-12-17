#include "DoorDetection.h"

DoorDetection::DoorDetection(){};
std::vector<Door> DoorDetection::FindDoorways(unsigned int door_thickness, unsigned int min_door_width, unsigned int max_door_width, World * world){
	std::vector<Door> newDoors, lastDoors;
	std::vector<Door>::iterator newDoorsIt, lastDoorsIt;
	std::vector<Door> doorways;

	Door aDoor;
	bool noLine = true;

	for(int x = 0; x < world->img->getWidth(); x++) {
		for(int y = 0; y < world->img->getHeight(); y++) {
		//find line segments
			if(world->img->getPixel(x, y) == 0 || (y == world->img->getHeight() - 1)) {
				if(noLine == false) {
				//end the last line segment
					aDoor.stop.x_pos = x;
					if(y == world->img->getHeight() - 1)
						aDoor.stop.y_pos = y;
					else
						aDoor.stop.y_pos = y - 1;
					//and save it
					newDoors.push_back(aDoor);
					noLine = true;
				}
			} else {
				if(noLine == true) {
					noLine = false;
					//start new line segment
					aDoor.start.x_pos = x;
					aDoor.start.y_pos = y;
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
				if(!(lastDoorsIt->getLength() > max_door_width) && !(lastDoorsIt->getLength() < min_door_width)) {
					//Is it on the edges of the map?
					if(lastDoorsIt->start.y_pos != 0 && lastDoorsIt->stop.y_pos != (world->img->getHeight() - 1)) {
						//Does the space behind become bigger and is it not to long?
						int stepback = 0;
						int leftStartPixelValue, leftEndPixelValue;
						int overStartPixelValue, underEndPixelValue;



						while(stepback < door_thickness){
							stepback++;

							leftStartPixelValue = world->img->getPixel(lastDoorsIt->start.x_pos - stepback, lastDoorsIt->start.y_pos);
							leftEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x_pos - stepback, lastDoorsIt->stop.y_pos);

							overStartPixelValue = world->img->getPixel(lastDoorsIt->start.x_pos - stepback, lastDoorsIt->start.y_pos - 1);
							underEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x_pos - stepback, lastDoorsIt->stop.y_pos + 1);


							if(leftStartPixelValue == 0 || leftEndPixelValue == 0) {
								//it is getting smaller (no door)
								break;
							} else if(overStartPixelValue != 0 || underEndPixelValue != 0) {
								//it is getting larger (door)
								doorways.push_back(*lastDoorsIt);
								break;
							}
						}
					}
				}
			}

			if(lastDoorsIt->stop.y_pos == newDoorsIt->stop.y_pos) {
				newDoorsIt++;
				lastDoorsIt++;
			} else if(lastDoorsIt->stop.y_pos > newDoorsIt->stop.y_pos) {
				newDoorsIt++;
			} else {
				lastDoorsIt++;
			}


			if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
				continue;

			if(lastDoorsIt->start.y_pos >= newDoorsIt->stop.y_pos) {
				newDoorsIt++;
			} else if(lastDoorsIt->stop.y_pos <= newDoorsIt->start.y_pos) {
				lastDoorsIt++;
			}
		}



		//Move new line segments to last line segments.
		lastDoors = newDoors;
		newDoors.clear();
	}


	return doorways;
}
