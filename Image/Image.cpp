#include "Image.h"

Image::Image (const Image *obj){
    height = obj->height;
    width = obj->width;
    maxValue = obj->maxValue;
    imageData = obj->imageData;
}


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

void Image::cleanupImage(void)
{
    std::vector<std::vector <int> > expand_map;
    int width = getWidth();
    int height = getHeight();
    int color = 0;

    //Might optimize later

    for(int i = 0; i < width; i++){
        std::vector<int> col(height);
        expand_map.push_back(col);
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            color = getPixel(x,y);
            if(color >= FREESPACE){    // mussy walls
                expand_map[x][y] = maxValue;
            }
            else if(color == STATION){      //docking station
                expand_map[x][y] = STATION;
            }
            else if(color >= ELEVATOR_ST && color <= ELEVATOR_SL){ // elevator
                elevatorExpansion(x, y, color);
            }
            else if(color == CUP){       // cups
                expand_map[x][y] = CUP;
            }
            else                                    // freespace
                expand_map[x][y] = WALL;
        }
    }

    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; y++){
            //img->setPixel(x,y,(expand_map[x][y] == 0) ? WHITE : BLACK);
            setPixel(x,y,expand_map[x][y]);
        }
    }
}

void Image::elevatorExpansion(int x, int y, int color) {
    // int expansion_factor = 6;
    bool elevator = true;
    for(int i = 0; i <= 1;i++){             // is it really an elevator ?
        for(int j = 0; j <= 1;j++){
            if(getPixel(x+j, y+i) == color){
            }
            else {
                elevator = false;
            }
            if ((x + 200) >= getWidth() || (y + 200) >= getHeight()) {
                elevator = false;
            }
        }
    }
    if(elevator){                           // yes it was
        for(int i = 0; getPixel(x + i - 6, y - 6) != WALL; i++){
            for(int j = 0; getPixel(x + i - 5, y + j - 5) != WALL; j++){
                setPixel(x+i-5, y+j-5, WALL);
            }
        }
    }
    else {                                  // no it was not
        setPixel(x, y, WALL);
    }
}

void Image::cleanupImageRobot(void)
{
    std::vector<std::vector <int> > expand_map;
    int width = getWidth();
    int height = getHeight();
    int color = 0;

    //Might optimize later

    for(int i = 0; i < width; i++){
        std::vector<int> col(height);
        expand_map.push_back(col);
    }

    for(int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            color = getPixel(x,y);
            if(color >= 1){    // mussy walls
                expand_map[x][y] = maxValue;
            }
            else                                    // freespace
                expand_map[x][y] = WALL;
        }
    }

    for(int x = 0; x < width; ++x){
        for(int y = 0; y < height; y++){
            //img->setPixel(x,y,(expand_map[x][y] == 0) ? WHITE : BLACK);
            setPixel(x,y,expand_map[x][y]);
        }
    }
}

