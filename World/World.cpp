/*
 * World.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#include "World.h"
#include "../Point/Point.h"
#include <algorithm>

#define OFFLOADING_LEFT_X 2393
#define OFFLOADING_LEFT_Y 1303

#define OFFLOADING_RIGHT_X 2872
#define OFFLOADING_RIGHT_Y 1299


World::World(Image *rawImg) {
    img = new Image(rawImg);
}


void World::Wavefront_offloading() {
    unsigned int value1 = 1;		//used for coloring the waves
    unsigned int value2 = 1;		//used for coloring the waves

    img->setPixel(OFFLOADING_LEFT_X, OFFLOADING_LEFT_Y, value1);
    img->setPixel(OFFLOADING_RIGHT_X, OFFLOADING_RIGHT_Y, value2);

    std::queue<Point> brushfires1;
    std::queue<Point> brushfires2;


    //check adjacent pixels
    //    Brushfire Qstart(locX, locY); //Brushfire objects (basically just a point or pixel)
    brushfires1.push(Point(OFFLOADING_LEFT_X, OFFLOADING_LEFT_Y));      //brushfires is a queue of Brushfire objects
    brushfires2.push(Point(OFFLOADING_RIGHT_X, OFFLOADING_RIGHT_Y));      //brushfires is a queue of Brushfire objects

    while(!brushfires1.empty() || !brushfires2.empty()) {

        if(!brushfires1.empty()){
            Point currentBrushfire1 = brushfires1.front();
            brushfires1.pop();
            value1 = img->getPixel(currentBrushfire1.x, currentBrushfire1.y) + 1;
            //check and set all adjacent pixels (8 point connectivity)
            for(signed int x_add = -1; x_add < 2; x_add++) {
                for(signed int y_add = -1; y_add < 2; y_add++) {

                    //check for out of bounds, skip if it is
                    if(outOfBounds(currentBrushfire1.x + x_add, currentBrushfire1.y + y_add)) {
                        continue;
                    }

                    unsigned int check_value = img->getPixel(currentBrushfire1.x + x_add, currentBrushfire1.y + y_add);

                    if(check_value > value1) {
                        Point nextFire(currentBrushfire1.x + x_add, currentBrushfire1.y + y_add);
                        brushfires1.push(nextFire);

                        img->setPixel(currentBrushfire1.x + x_add, currentBrushfire1.y + y_add, value1);
                    }
                }
            }
        }

        if(!brushfires2.empty()){
            Point currentBrushfire2 = brushfires2.front();
            brushfires2.pop();
            value2 = img->getPixel(currentBrushfire2.x, currentBrushfire2.y) + 1;
            //check and set all adjacent pixels (8 point connectivity)
            for(signed int x_add = -1; x_add < 2; x_add++) {
                for(signed int y_add = -1; y_add < 2; y_add++) {

                    //check for out of bounds, skip if it is
                    if(outOfBounds(currentBrushfire2.x + x_add, currentBrushfire2.y + y_add)) {
                        continue;
                    }

                    unsigned int check_value = img->getPixel(currentBrushfire2.x + x_add, currentBrushfire2.y + y_add);

                    if(check_value > value2) {
                        Point nextFire(currentBrushfire2.x + x_add, currentBrushfire2.y + y_add);
                        brushfires2.push(nextFire);

                        img->setPixel(currentBrushfire2.x + x_add, currentBrushfire2.y + y_add, value2);
                    }
                }
            }
        }
    }
}

void World::Wavefront_navigation(Point start, Point stop) {

    if (img->getPixel(start.x,start.y) == 0) {
        std::cout << "Error - Started inside a wall at: " << start.x << "," << start.y << std::endl;
        exit(0);
    }

    unsigned int value = 1;		//used for coloring the waves
    img->setPixel(start.x, start.y, value);

    std::queue<Point> brushfires;

    brushfires.push(start);      //brushfires is a queue of Brushfire objects

    while(!brushfires.empty()) {
        //get first brushfire in the queue and erase.
        Point currentBrushfire = brushfires.front();
        brushfires.pop();

        //get current pixel value
        value = img->getPixel(currentBrushfire.x, currentBrushfire.y) + 1;

        //check and set all adjacent pixels (8 point connectivity)
        for(signed int x_add = -1; x_add < 2; x_add++) {
            for(signed int y_add = -1; y_add < 2; y_add++) {

                // Stop when we reach stop-point
                if (currentBrushfire.x + x_add == stop.x && currentBrushfire.y + y_add == stop.y){
                    img->setPixel(currentBrushfire.x + x_add, currentBrushfire.y + y_add, value);
                    return;
                }
                //check for out of bounds, skip if it is
                if(outOfBounds(currentBrushfire.x + x_add, currentBrushfire.y + y_add)) {
                    continue;
                }

                //check if pixel value is larger and add new brushfire to queue (the value could be white or it could have already been checked
                //                                                                               both cases need to be updated because the new path is shorter)
                unsigned int check_value = img->getPixel(currentBrushfire.x + x_add, currentBrushfire.y + y_add);

                if(check_value > value) {
                    Point nextFire(currentBrushfire.x + x_add, currentBrushfire.y + y_add);
                    brushfires.push(nextFire);

                    img->setPixel(currentBrushfire.x + x_add, currentBrushfire.y + y_add, value);
                }
            }
        }
    }
    return;
}

bool comperator(Point a, Point b){
    return (a.x == b.x && a.y == b.y);
}

std::vector<Point> World::Wavefront_DoorScanner(Point &start, unsigned int door_color, unsigned int door_pixel_color){

    start.visited = true;

    // We start from value door_color to avoid stopping when the wavefront reaches the value.
    unsigned int value = door_color;		//used for coloring the waves
    img->setPixel(start.x, start.y, value);

    std::queue<Point> brushfires;
    std::vector<Point> door_px_points;

    brushfires.push(start);      //brushfires is a queue of Brushfire objects

    while(!brushfires.empty()) {
        //get first brushfire in the queue and erase.
        Point currentBrushfire = brushfires.front();
        brushfires.pop();

        //get current pixel value
        value = img->getPixel(currentBrushfire.x, currentBrushfire.y) + 1;

        //check and set all adjacent pixels (8 point connectivity)
        for(signed int x_add = -1; x_add < 2; x_add++) {
            for(signed int y_add = -1; y_add < 2; y_add++) {
                //check for out of bounds, skip if it is
                if(outOfBounds(currentBrushfire.x + x_add, currentBrushfire.y + y_add)) {
                    continue;
                }

                //check if pixel value is larger and add new brushfire to queue (the value could be white or it could have already been checked
                //                                                                               both cases need to be updated because the new path is shorter)
                unsigned int check_value = img->getPixel(currentBrushfire.x + x_add, currentBrushfire.y + y_add);
                if(check_value == door_pixel_color){
                    door_px_points.push_back(Point(currentBrushfire.x + x_add, currentBrushfire.y + y_add));
                }

                if(check_value > value && !(check_value == door_color)) { // Stop when we hit door
                    Point nextFire(currentBrushfire.x + x_add, currentBrushfire.y + y_add);
                    brushfires.push(nextFire);

                    img->setPixel(currentBrushfire.x + x_add, currentBrushfire.y + y_add, value);
                }
            }
        }
    }


    std::vector<Point>::iterator it = std::unique(door_px_points.begin(), door_px_points.end(), comperator);
    door_px_points.resize(std::distance(door_px_points.begin(), it));
    return door_px_points;
}


void World::WallExpansion() {
    int expansion_factor = 4;
    std::vector<std::vector <int> > expand_map;

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
                            expand_map[x+j][y+i] = 0;
                        }
                        if(y+i <= img->getHeight()-1 && x-j >= 0){
                            expand_map[x-j][y+i] = 0;
                        }
                        if(y-i >= 0 && x-j >= 0 ){
                            expand_map[x-j][y-i] = 0;
                        }
                        if(y-i >= 0 && x+j <= img->getWidth()-1 ){
                            expand_map[x+j][y-i] = 0;
                        }
                    }
                }
            }
            else if(img->getPixel(x,y) == 150){
                expand_map[x][y] = 150;
            }
            else{
                expand_map[x][y] = img->maxValue;
            }
        }
    }

    for(int x = 0; x < img->getWidth(); ++x){
        for(int y = 0; y < img->getHeight(); y++){
            img->setPixel(x,y,expand_map[x][y]);
        }
    }
}

bool World::outOfBounds(signed int x, signed int y)
{
    if(x <= 0 || x >= img->getWidth())
        return true;
    else if(y <= 0 || y >= img->getHeight())
        return true;
    else
        return false;
}
