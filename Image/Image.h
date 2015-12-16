#ifndef IMAGE_H_
#define IMAGE_H_

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <algorithm> // std::reverse

class Image {
public:
	Image();
	int getWidth();
	int getHeight();
	int getMaxValue();
	std::string getVersion();
	int getPixel(int width, int height);
	void setPixel(const int width, const int height, const int value);
	void createVector(std::vector<std::vector<int>> &image);
	void loadImage(const std::string filename);
	void saveImage(std::string filename);
	std::vector<std::vector<int>> imageData;
	std::fstream imageFile;
	int width;
	int height;
	int maxValue;
	std::string inputLine;
	std::string version;
	std::stringstream ss;
};


#endif /* IMAGE_H_ */

