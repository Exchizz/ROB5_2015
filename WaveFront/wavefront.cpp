#include "Wavefront.h"


Wavefront::Wavefront(void)
{
}

Wavefront::Wavefront(rw::sensor::Image *ws)
{
	workspace = ws;
	ws_height = workspace->getHeight();
	ws_width = workspace->getWidth();
}

Wavefront::~Wavefront(void)
{
}


void Wavefront::createWavefront(signed int locX, signed int locY)
{
    unsigned int value = 1;		//used for coloring the waves
	workspace->setPixel16U(locX, locY, value);

	//check adjacent pixels
    Brushfire Qstart(locX, locY); //Brushfire objects (basically just a point or pixel)
    brushfires.push(Qstart);      //brushfires is a queue of Brushfire objects

	while(!brushfires.empty()) {
		//get first brushfire in the queue and erase.
		Brushfire currentBrushfire = brushfires.front();
		brushfires.pop();

		//get current pixel value
		value = workspace->getPixelValuei(currentBrushfire.x, currentBrushfire.y, 0) + 1;

        //check and set all adjacent pixels (8 point connectivity)
        for(signed int x_add = -1; x_add < 2; x_add++) {
            for(signed int y_add = -1; y_add < 2; y_add++) {

                //check for out of bounds, skip if it is
				if(outOfBounds(currentBrushfire.x + x_add, currentBrushfire.y + y_add)) {
					continue;
				}

                //check if pixel value is larger and add new brushfire to queue (the value could be white or it could have already been checked
//                                                                               both cases need to be updated because the new path is shorter)
                unsigned int check_value = workspace->getPixelValuei(currentBrushfire.x + x_add, currentBrushfire.y + y_add, 0);

				if(check_value > value) {
					Brushfire nextFire(currentBrushfire.x + x_add, currentBrushfire.y + y_add);
					brushfires.push(nextFire);

					workspace->setPixel16U(currentBrushfire.x + x_add, currentBrushfire.y + y_add, value);
				}
			}
		}
	}
}



bool Wavefront::outOfBounds(signed int x, signed int y)
{
	if(x < 0 || x >= ws_width)
		return true;
	else if(y < 0 || y >= ws_height)
		return true;
	else
		return false;
}



//be aware of this !!!
Wavefront::Brushfire::Brushfire()
{
	this->x = 0;
	this->y = 0;
}

//be aware of this !!!
Wavefront::Brushfire::Brushfire(int x, int y)
{
	this->x = x;
	this->y = y;
}

