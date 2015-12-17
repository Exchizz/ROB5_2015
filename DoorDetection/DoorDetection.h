#pragma once

#include "../Point/Point.h"
#include "../Door/Door.h"
#include "../World/World.h"
#include <vector>
//#include <cstdlib>
/*
class LineSegment {
public:
	LineSegment();
	Point start;
	Point end;

	unsigned int getLength() {
		int lengthX = abs(end.x_pos-start.x_pos);
		int lengthY = abs(end.y_pos-start.y_pos);
		if(lengthX > lengthY)
			return lengthX;
		else
			return lengthY;
	}
};
*/

class DoorDetection {
public:
	DoorDetection();
	std::vector<Door> FindDoorways(unsigned int door_thickness, unsigned int min_door_width, unsigned int max_door_width, World* world);
};

