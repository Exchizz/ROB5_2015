#include "Image.h"

int Image::getMaxValue() {
	return maxValue;
}
std::string Image::getVersion() {
	return version;
}
void Image::saveImage(std::string filename) {
	imageFile.open(filename.c_str(), std::ios::out);
	if (!imageFile.is_open()) {
		std::cout << "Unable to create \"" + filename + "\"" << std::endl;
		exit(0);
	}
	imageFile << "P2\n";
	imageFile << "# THE BEER-WARE LICENSE (Revision 42)\n";
	imageFile << width << " " << height << "\n";
	imageFile << maxValue << "\n";
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			imageFile << getPixel(x, y) << " ";
		}
		imageFile << std::endl;
	}
	imageFile.close();
}
void Image::loadImage(const std::string filename) {
	imageFile.open(filename.c_str());
	if (!imageFile.is_open()) {
		std::cout << "Unable to open \"" + filename + "\"" << std::endl;
		exit(0);
	}
// First line : version
	getline(imageFile, inputLine);
	if (inputLine.compare("P2") != 0) {
		std::cout << "Version NOT P2, be aware code might not work as expected"
				<< std::endl;
		;
	} else
		std::cout << "Version : " << inputLine << std::endl;
	this->version = inputLine;
// Second line : comment
	getline(imageFile, inputLine);
	std::cout << "Comment : " << inputLine << std::endl;
// Third line : size
	getline(imageFile, inputLine);
	ss << inputLine;
	ss >> width >> height;
	std::cout << width << " columns and " << height << " rows" << std::endl;
// fourth line : max value
	getline(imageFile, inputLine);
	ss.clear();
	ss << inputLine;
	ss >> maxValue;

	std::cout << "max value : " << maxValue << std::endl;

	ss.clear();
	ss << imageFile.rdbuf();
//Creates imageData-vector
	createVector (imageData);
//Put image into 2-D vectors

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			int value;
			ss >> value;
			setPixel(x, y, value);
		}
	}
	imageFile.close();
}
Image::Image() {
	height = 0;
	width = 0;
	maxValue = 0;
}
int Image::getWidth() {
	return width;
}
int Image::getHeight() {
	return height;
}
void Image::createVector(std::vector<std::vector<int> > &image) {
	//image.reserve(width);

	for (int i = 0; i < width; ++i) {
		std::vector<int> temp(height);
		image.push_back(temp);
		//image[i] = temp;
	}
}
int Image::getPixel(int width, int height) {
	if (width >= this->width) {
		std::cout << "Requested width: \"" << width << "\" max width is: \""
				<< this->width - 1 << "\"" << std::endl;
		exit(0);
	}
	if (height >= this->height) {
		std::cout << "Requested height: \"" << height << "\" max height is: \""
				<< this->height - 1 << "\"" << std::endl;
		exit(0);
	}
	return imageData[width][height];
}
void Image::setPixel(const int width, const int height, const int value) {
	if (width >= this->width) {
		std::cout << "Requested width: \"" << width << "\" max width is: \""
				<< this->width - 1 << "\"" << std::endl;
		exit(0);
	}
	if (height >= this->height) {
		std::cout << "Requested height: \"" << height << "\" max height is: \""
				<< this->height - 1 << "\"" << std::endl;
		exit(0);
	}
	imageData[width][height] = value;
}

