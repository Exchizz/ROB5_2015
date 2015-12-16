/*
 * World.h
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#ifndef WORLD_WORLD_H_
#define WORLD_WORLD_H_

#include "../Image/Image.h"
#include "../Point.h"
#include <queue>

class World {
public:
	Image *img;

	World(Image *img);
	void WallExpansion();
    void Wavefront(Point start);
    void Wavefront(Point start, Point stop);

    void Wavefront(Point start, Point stop, bool doStop);


    std::queue<Point> brushfires;
    bool outOfBounds(signed int x, signed int y);
};

#endif /* WORLD_WORLD_H_ */
