#pragma once
#include "../Point/Point.h"
#include <cstdlib>
#include <vector>

enum HitPoint { NONE, PX1, PX2};

class Door {
private:

public:
	Point start;
	Point stop;
    Point px1;
    Point px2;

	int cover = false;

	HitPoint FirstPoint = NONE;

	Door();
	Door(Point start, Point stop);
	std::vector<Door> children;
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
