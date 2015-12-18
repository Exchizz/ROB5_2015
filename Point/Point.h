#pragma once

class Point {
public:
	signed int x_pos = 0;
	signed int y_pos = 0;
    bool visited = false;
	Point();
        Point(signed int x, signed int y);
};

