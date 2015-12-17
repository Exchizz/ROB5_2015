#pragma once
#include "../Point/Point.h"
#include <cstdlib>

class Door {
private:

public:
	Point start;
	Point stop;
    Point px1;
    Point px2;
    bool px1_visited = false;
    bool px2_visited = false;
	Door();
	Door(Point start, Point stop);
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
