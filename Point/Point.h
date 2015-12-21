#pragma once

class Point {
public:
    double x = 0;
    double y = 0;
    bool visited = false;

    bool operator==(Point rhs){
    	 return (this->x == rhs.x && this->y == rhs.y);
    }
	Point();
        Point(double x, double y);
};
