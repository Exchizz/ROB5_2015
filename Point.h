#pragma once

class Point {
public:
	signed int x_pos;
	signed int y_pos;
        Point(void);
        Point(signed int x, signed int y): x_pos(x), y_pos(y){};

};

