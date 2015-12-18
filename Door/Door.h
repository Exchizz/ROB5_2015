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
                int lengthX = abs(stop.x_pos-start.x_pos);
                int lengthY = abs(stop.y_pos-start.y_pos);
                if(lengthX > lengthY)
                        return lengthX;
                else
                        return lengthY;
        }
};
