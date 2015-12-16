/*
 * World.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#include "World.h"


World::World(Image *rawImg) {
	img = new Image(rawImg);

	//img->imageData = rawImg->imageData;
	// TODO Auto-generated constructor stub
}


void World::Wavefront(Point start) {
    Wavefront(start, Point(0,0), false);
}
void World::Wavefront(Point start, Point stop) {
    Wavefront(start, stop, true);
}


void World::Wavefront(Point start, Point stop, bool doStop) {

    unsigned int value = 1;		//used for coloring the waves
    img->setPixel(start.x_pos, start.y_pos, value);

    //check adjacent pixels
                                   //    Brushfire Qstart(locX, locY); //Brushfire objects (basically just a point or pixel)
    brushfires.push(start);      //brushfires is a queue of Brushfire objects

    while(!brushfires.empty()) {
        //get first brushfire in the queue and erase.
        Point currentBrushfire = brushfires.front();
        brushfires.pop();

        //get current pixel value
        value = img->getPixel(currentBrushfire.x_pos, currentBrushfire.y_pos) + 1;

        //check and set all adjacent pixels (8 point connectivity)
        for(signed int x_add = -1; x_add < 2; x_add++) {
            for(signed int y_add = -1; y_add < 2; y_add++) {

                // Stop when we reach stop-point
                if(doStop){
                    if (currentBrushfire.x_pos + x_add == stop.x_pos && currentBrushfire.y_pos + y_add == stop.y_pos){
                        return;
                    }
                }

                //check for out of bounds, skip if it is
                if(outOfBounds(currentBrushfire.x_pos + x_add, currentBrushfire.y_pos + y_add)) {
                    continue;
                }

                //check if pixel value is larger and add new brushfire to queue (the value could be white or it could have already been checked
//                                                                               both cases need to be updated because the new path is shorter)
                unsigned int check_value = img->getPixel(currentBrushfire.x_pos + x_add, currentBrushfire.y_pos + y_add);

                if(check_value > value) {
                    Point nextFire(currentBrushfire.x_pos + x_add, currentBrushfire.y_pos + y_add);
                    brushfires.push(nextFire);

                    img->setPixel(currentBrushfire.x_pos + x_add, currentBrushfire.y_pos + y_add, value);
                }
            }
        }
    }

}

void World::WallExpansion() {
		int expansion_factor = 4;
		std::vector<std::vector <int> > expand_map;


		//Might optimize later

		for(int i = 0; i < img->getWidth(); i++){
			std::vector<int> col(img->getHeight());
			expand_map.push_back(col);
		}

		for(int x = 0; x < img->getWidth(); x++){
			for(int y = 0; y < img->getHeight(); y++){
				if(img->getPixel(x,y) <= 50){ // Retter 50 senere
					for(int i = 0; i <= expansion_factor;i++){
						for(int j = 0; j <= expansion_factor;j++){
							if(y+i <= img->getHeight()-1 && x+j <= img->getWidth()-1 ){
								expand_map[x+j][y+i] = 1;
							}
							if(y+i <= img->getHeight()-1 && x-j >= 0){
								expand_map[x-j][y+i] = 1;
							}
							if(y-i >= 0 && x-j >= 0 ){
								expand_map[x-j][y-i] = 1;
							}
							if(y-i >= 0 && x+j <= img->getWidth()-1 ){
								expand_map[x+j][y-i] = 1;
							}
						}
					}
				}else if(expand_map[x][y] != 1){
						expand_map[x][y] = img->maxValue;
				}
			}
		}

		for(int x = 0; x < img->getWidth(); ++x){
			for(int y = 0; y < img->getHeight(); y++){
				//img->setPixel(x,y,(expand_map[x][y] == 0) ? WHITE : BLACK);
				img->setPixel(x,y,expand_map[x][y]);
			}
		}
}


bool World::outOfBounds(signed int x, signed int y)
{
    if(x < 0 || x >= img->getWidth())
        return true;
    else if(y < 0 || y >= img->getHeight())
        return true;
    else
        return false;
}
