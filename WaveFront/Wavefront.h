#pragma once

#include <iostream>
#include <queue>
#include "Image.hpp"
//#include "Brushfire.h"

class Wavefront
{
private:
	struct Brushfire {
		Brushfire();
		Brushfire(int, int);
		    
		int x;
		int y;
	};

	std::queue<Brushfire> brushfires;

	rw::sensor::Image *workspace;
	
	bool outOfBounds(int, int);
	
	int ws_width;
	int ws_height;
	
public:
	Wavefront(void);
	Wavefront(rw::sensor::Image *ws);
	~Wavefront(void);

	void createWavefront(int, int);
};

