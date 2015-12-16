/*
 * World.h
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#ifndef WORLD_WORLD_H_
#define WORLD_WORLD_H_

#include "../Image/Image.h"

class World {
public:
	Image *img;

	World(Image *img);
	void WallExpansion();
};

#endif /* WORLD_WORLD_H_ */
