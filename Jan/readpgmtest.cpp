#include <iostream>
#include <vector>
#include "Image.hpp"
#include "PPMLoader.hpp"
#include <cstdlib>

#define DOORWIDTH_MAX 50
#define DOORWIDTH_MIN 5
#define DOORTHICKNESS 8

using namespace rw::sensor;
using namespace rw::loaders;

class workspace {
public:
	workspace(rw::sensor::Image *chosenWS) {
		height = chosenWS->getHeight();
		width = chosenWS->getWidth();

		ws = new unsigned char*[width];
		for(int x = 0; x < width; ++x)
		    ws[x] = new unsigned char[height];

		for(int x = 0; x < width; ++x) {
		    for(int y = 0; y < height; ++y) {
				ws[x][y] = chosenWS->getPixelValuei(x, y, 0);
		    }
		}
	}

	void setPixelValue(int x, int y, unsigned char value) {
		ws[x][y] = value;
	}

	unsigned char getPixelValue(int x, int y) {
		return ws[x][y];
	}

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

private:
	unsigned char **ws;
	int width, height;
};


struct point {
	int x, y;
};

struct lineSegment {
	point start;
	point end;

	int getLength() {
		int lengthX = abs(end.x-start.x);
		int lengthY = abs(end.y-start.y);
		if(lengthX > lengthY)
			return lengthX;
		else
			return lengthY;
	}
};



void findDoorways(workspace* ws, std::vector<lineSegment> &doorways) {
	std::vector<lineSegment> newLineSegments, lastLineSegments;
	std::vector<lineSegment>::iterator newLineSegmentsIt, lastLineSegmentsIt;

	lineSegment aLineSegment;
	bool noLine = true;

	for(int x = 0; x < ws->getWidth(); x++) {
		for(int y = 0; y < ws->getHeight(); y++) {
		//find line segments
			if(ws->getPixelValue(x, y) == 0 || (y == ws->getHeight() - 1)) {
				if(noLine == false) {
				//end the last line segment
					aLineSegment.end.x = x;
					if(y == ws->getHeight() - 1)
						aLineSegment.end.y = y;
					else
						aLineSegment.end.y = y - 1;
				//and save it
					newLineSegments.push_back(aLineSegment);
					noLine = true;
				}
			}
			else {
				if(noLine == true) {
					noLine = false;
				//start new line segment
					aLineSegment.start.x = x;
					aLineSegment.start.y = y;
				}
			}
		}
	//Compare line segments in queue.
		newLineSegmentsIt = newLineSegments.begin();
		lastLineSegmentsIt = lastLineSegments.begin();
		
		while(lastLineSegmentsIt != lastLineSegments.end() && newLineSegmentsIt != newLineSegments.end()) {
			//It is a door?
			if(lastLineSegmentsIt->getLength() < newLineSegmentsIt->getLength()) {
				//is it to long to be a door?
				if(!(lastLineSegmentsIt->getLength() > DOORWIDTH_MAX) && !(lastLineSegmentsIt->getLength() < DOORWIDTH_MIN)) {
					//Is it on the edges of the map?
					if(lastLineSegmentsIt->start.y != 0 && lastLineSegmentsIt->end.y != (ws->getHeight() - 1)) {
						//Does the space behind become bigger and is it not to long?
						int stepback = 0;
						int leftStartPixelValue, leftEndPixelValue;
						int overStartPixelValue, underEndPixelValue;

						while(stepback < DOORTHICKNESS) {
							stepback++;
							
							leftStartPixelValue = ws->getPixelValue(lastLineSegmentsIt->start.x - stepback, lastLineSegmentsIt->start.y);
							leftEndPixelValue = ws->getPixelValue(lastLineSegmentsIt->end.x - stepback, lastLineSegmentsIt->end.y);

							overStartPixelValue = ws->getPixelValue(lastLineSegmentsIt->start.x - stepback, lastLineSegmentsIt->start.y - 1);
							underEndPixelValue = ws->getPixelValue(lastLineSegmentsIt->end.x - stepback, lastLineSegmentsIt->end.y + 1);

							if(leftStartPixelValue == 0 || leftEndPixelValue == 0) {
								//it is getting smaller (no door)
								break;
							}
							else if(overStartPixelValue != 0 || underEndPixelValue != 0) {
								//it is getting larger (door)
								doorways.push_back(*lastLineSegmentsIt);
								break;
							}

							//std::cout << "Location: (" << lastLineSegmentsIt->start.x << ", " << lastLineSegmentsIt->start.y << ")";
							//std::cout << " : (" << lastLineSegmentsIt->end.x << ", " << lastLineSegmentsIt->end.y << ")";
							//std::cout << "   length: " << lastLineSegmentsIt->getLength() << std::endl;
						}
					}
				}
			}
				
			if(lastLineSegmentsIt->end.y == newLineSegmentsIt->end.y) {
				newLineSegmentsIt++;
				lastLineSegmentsIt++;
			}
			else if(lastLineSegmentsIt->end.y > newLineSegmentsIt->end.y) {
				newLineSegmentsIt++;
			}
			else {
				lastLineSegmentsIt++;
			}

			if(lastLineSegmentsIt == lastLineSegments.end() || newLineSegmentsIt == newLineSegments.end())
				continue;

			if(lastLineSegmentsIt->start.y >= newLineSegmentsIt->end.y) {
				newLineSegmentsIt++;
			}
			else if(lastLineSegmentsIt->end.y <= newLineSegmentsIt->start.y) {
				lastLineSegmentsIt++;
			}
		}

	//Move new line segments to last line segments.
			lastLineSegments = newLineSegments;
			newLineSegments.clear();
	}
}



int main(int argc, char** argv) {
    std::string filename(argv[1]);

    std::cout << "loading image..." << std::endl;
    Image* img = PPMLoader::load(filename);

	std::cout << "Making workspace" << std::endl;
	workspace* ws = new workspace(img);
	
	std::cout << ws->getWidth() << ", " << ws->getHeight() << std::endl;
	std::vector<lineSegment> doorways;

	std::cout << "Finding Doorways" << std::endl;
	findDoorways(ws, doorways);

	std::vector<lineSegment>::iterator doorwaysIt;

	for(std::vector<lineSegment>::iterator doorwaysIt = doorways.begin(); doorwaysIt != doorways.end(); doorwaysIt++) {
		for(int y = doorwaysIt->start.y; y <= doorwaysIt->end.y; ++y) {
			img->setPixel8U(doorwaysIt->start.x, y, 127);
		}
		//std::cout << "There was a door: (" << doorwaysIt.start.x << ", " << doorwaysIt.start.y << ")";
		//std::cout << " ; (" << doorwaysIt.end.x << ", " << doorwaysIt.end.y << ")" << std::endl;
	}



    std::cout << "saving image..." << std::endl;
    // save image
    img->saveAsPGM("testout.pgm");
        
    // cleanup
    delete img;
}

