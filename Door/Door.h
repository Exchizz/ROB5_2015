#pragma once
#include "../Point/Point.h"
#include <cstdlib>
#include <vector>

class Door {
private:

public:
	Point start;
	Point stop;
    Point px1;
    Point px2;
	Door();
	Door(Point start, Point stop);
	std::vector<Door> adjacent;
	unsigned int id;

        unsigned int getLength() {
                int lengthX = abs(stop.x-start.x);
                int lengthY = abs(stop.y-start.y);
                if(lengthX > lengthY)
                        return lengthX;
                else
                        return lengthY;
        }
};
