#pragma once

#include "../Point/Point.h"
#include "../Door/Door.h"
#include "../World/World.h"
#include <vector>
//#include <cstdlib>
/*
class LineSegment {
public:#define DOORWIDTH_MAX 50
#define DOORWIDTH_MIN 5
#define DOORTHICKNESS_MAX 20
#define DOORTHICKNESS_MIN 2
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

#define DOORWIDTH_MAX 50
#define DOORWIDTH_MIN 5
#define DOORTHICKNESS_MAX 20
#define DOORTHICKNESS_MIN 2

class DoorDetection {
public:
	DoorDetection();
    std::vector<Door> FindDoorways(const World *world);
private:
    void FindHorizontalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways);
    void FindVerticalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways);
    void drawDoors(World* world, std::vector<Door> &doorways);
};

