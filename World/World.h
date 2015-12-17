/*
 * World.h
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#ifndef WORLD_WORLD_H_
#define WORLD_WORLD_H_

#include "../Image/Image.h"
#include "../Point/Point.h"
#include <queue>
#include <list>
#include "../Door/Door.h"


class World {
public:
	Image *img;

	World(Image *img);
	void WallExpansion();
    void Wavefront_navigation(Point start, Point stop);
    void Wavefront_offloading(Point start1, Point start2);

    std::vector<Point> Wavefront_DoorScanner(Point start, unsigned int stop_color, unsigned int door_pixel_color);
    //void Wavefront(Point start, Point stop);

    //void Wavefront(Point start, Point stop, bool doStop);

    bool outOfBounds(signed int x, signed int y);

};

#endif /* WORLD_WORLD_H_ */
