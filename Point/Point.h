#pragma once

class Point {
public:
	signed int x_pos;
	signed int y_pos;
    bool visited = false;
	Point();
        Point(signed int x, signed int y);
};

