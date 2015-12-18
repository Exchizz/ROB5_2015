#include "DoorDetection.h"


DoorDetection::DoorDetection(){}

std::vector<Door>  DoorDetection::FindDoorways(const World* world){
    std::vector<Door> doorways;
    World* doorWorld_ptr = new World(world->img);

    FindVerticalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX / 3, DOORWIDTH_MIN, DOORWIDTH_MAX / 3, doorWorld_ptr, doorways);
    drawDoors(doorWorld_ptr, doorways);
    FindHorizontalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX, DOORWIDTH_MIN, DOORWIDTH_MAX -2, doorWorld_ptr, doorways);
    drawDoors(doorWorld_ptr, doorways);
    FindVerticalDoorways(DOORTHICKNESS_MIN, DOORTHICKNESS_MAX, DOORWIDTH_MIN, DOORWIDTH_MAX, doorWorld_ptr, doorways);

    return doorways;
}

void DoorDetection::drawDoors(World* world, std::vector<Door> &doorways)
{
    for(auto doorwaysIt : doorways) {
        for(int x = doorwaysIt.start.x_pos; x <= doorwaysIt.stop.x_pos; ++x) {
            for(int y = doorwaysIt.start.y_pos; y <= doorwaysIt.stop.y_pos; ++y) {
                world->img->setPixel(x, y, 0);
            }
        }
    }
}

void DoorDetection::FindHorizontalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways){
    std::vector<Door> newDoors, lastDoors;
    std::vector<Door>::iterator newDoorsIt, lastDoorsIt;

    Door aDoor;
    bool noLine = true;

    for(int y = 0; y < world->img->getHeight(); y++) {
        for(int x = 0; x < world->img->getWidth(); x++) {
            //find line segments
            if(world->img->getPixel(x, y) == 0 || (x == world->img->getWidth() - 1)) {
                if(noLine == false) {
                    //end the last line segment
                    aDoor.stop.y_pos = y;
                    if(x == world->img->getWidth() - 1)
                        aDoor.stop.x_pos = x;
                    else
                        aDoor.stop.x_pos = x - 1;
                    //and save it
                    newDoors.push_back(aDoor);
                    noLine = true;
                }
            } else {
                if(noLine == true) {
                    noLine = false;
                    //start new line segment
                    aDoor.start.x_pos = x;
                    aDoor.start.y_pos = y;
                }
            }
        }
        //Compare line segments in queue.
        newDoorsIt = newDoors.begin();
        lastDoorsIt = lastDoors.begin();

        while(lastDoorsIt != lastDoors.end() && newDoorsIt != newDoors.end()) {
            //It is a door?
            if(lastDoorsIt->getLength() < newDoorsIt->getLength()) {
                //is it to long to be a door?
                if(!(lastDoorsIt->getLength() > doorWidth_max) && !(lastDoorsIt->getLength() < doorWidth_min)) {
                    //Is it on the edges of the map?
                    if(lastDoorsIt->start.x_pos != 0 && lastDoorsIt->stop.x_pos != (world->img->getWidth() - 1)) {
                        //Does the space behind become bigger and is it not to long?
                        int stepback = 0;
                        int overStartPixelValue, underEndPixelValue;
                        bool keepLooping = true;

                        while(stepback < doorThickness_max && keepLooping == true) {
                            stepback++;

                            for(int i = lastDoorsIt->start.x_pos; i <= lastDoorsIt->stop.x_pos; i++){
                                if(world->img->getPixel(i, lastDoorsIt->start.y_pos - stepback) == 0) {
                                    keepLooping = false;
                                    break;
                                }
                            }

                            overStartPixelValue = world->img->getPixel(lastDoorsIt->start.x_pos - 1, lastDoorsIt->start.y_pos - stepback);
                            underEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x_pos + 1, lastDoorsIt->stop.y_pos - stepback);

                            if(overStartPixelValue != 0 || underEndPixelValue != 0) {
                                //it is getting larger (door)
                                if(stepback >= doorThickness_min)
                                    doorways.push_back(*lastDoorsIt);
                                keepLooping = false;
                            }
                        }
                    }
                }
            }

            //Are the line segments adjacent to each other?
            do {
                //std::cout << "I am looping" << std::endl;
                if(lastDoorsIt->stop.x_pos == newDoorsIt->stop.x_pos) {
                    newDoorsIt++;
                    lastDoorsIt++;
                }
                else if(lastDoorsIt->stop.x_pos > newDoorsIt->stop.x_pos) {
                    newDoorsIt++;
                }
                else {
                    lastDoorsIt++;
                }

                if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
                    break;

                if(lastDoorsIt->start.x_pos >= newDoorsIt->stop.x_pos) {
                    newDoorsIt++;
                }
                else if(lastDoorsIt->stop.x_pos <= newDoorsIt->start.x_pos) {
                    lastDoorsIt++;
                }
            } while(lastDoorsIt->stop.x_pos < newDoorsIt->start.x_pos || lastDoorsIt->start.x_pos > newDoorsIt->stop.x_pos);

            if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
                continue;
        }

        //Move new line segments to last line segments.
        lastDoors = newDoors;
        newDoors.clear();
    }
}

void DoorDetection::FindVerticalDoorways(unsigned int doorThickness_min , unsigned int doorThickness_max, unsigned int doorWidth_min, unsigned int doorWidth_max, World* world, std::vector<Door> &doorways){
    std::vector<Door> newDoors, lastDoors;
    std::vector<Door>::iterator newDoorsIt, lastDoorsIt;

    Door aDoor;
    bool noLine = true;

    for(int x = 0; x < world->img->getWidth(); x++) {
        for(int y = 0; y < world->img->getHeight(); y++) {
            //find line segments
            if(world->img->getPixel(x, y) == 0 || (y == world->img->getHeight() - 1)) {
                if(noLine == false) {
                    //end the last line segment
                    aDoor.stop.x_pos = x;
                    if(y == world->img->getHeight() - 1)
                        aDoor.stop.y_pos = y;
                    else
                        aDoor.stop.y_pos = y - 1;
                    //and save it
                    newDoors.push_back(aDoor);
                    noLine = true;
                }
            } else {
                if(noLine == true) {
                    noLine = false;
                    //start new line segment
                    aDoor.start.x_pos = x;
                    aDoor.start.y_pos = y;
                }
            }
        }
        //Compare line segments in queue.
        newDoorsIt = newDoors.begin();
        lastDoorsIt = lastDoors.begin();

        while(lastDoorsIt != lastDoors.end() && newDoorsIt != newDoors.end()) {
            //It is a door?
            if(lastDoorsIt->getLength() < newDoorsIt->getLength()) {
                //is it to long to be a door?
                if(!(lastDoorsIt->getLength() > doorWidth_max) && !(lastDoorsIt->getLength() < doorWidth_min)) {
                    //Is it on the edges of the map?
                    if(lastDoorsIt->start.y_pos != 0 && lastDoorsIt->stop.y_pos != (world->img->getHeight() - 1)) {
                        //Does the space behind become bigger and is it not to long?
                        int stepback = 0;
                        int overStartPixelValue, underEndPixelValue;
                        bool keepLooping = true;

                        while(stepback < doorThickness_max && keepLooping == true) {
                            stepback++;

                            for(int i = lastDoorsIt->start.y_pos; i <= lastDoorsIt->stop.y_pos; i++){
                                if(world->img->getPixel(lastDoorsIt->start.x_pos - stepback, i) == 0) {
                                    keepLooping = false;
                                    break;
                                }
                            }

                            overStartPixelValue = world->img->getPixel(lastDoorsIt->start.x_pos - stepback, lastDoorsIt->start.y_pos - 1);
                            underEndPixelValue = world->img->getPixel(lastDoorsIt->stop.x_pos - stepback, lastDoorsIt->stop.y_pos + 1);

                            if(overStartPixelValue != 0 || underEndPixelValue != 0) {
                                //it is getting larger (door)
                                doorways.push_back(*lastDoorsIt);
                                keepLooping = false;
                            }
                        }
                    }
                }
            }

            //Are the line segments adjacent to each other?
            do {
                //std::cout << "I am looping" << std::endl;
                if(lastDoorsIt->stop.y_pos == newDoorsIt->stop.y_pos) {
                    newDoorsIt++;
                    lastDoorsIt++;
                }
                else if(lastDoorsIt->stop.y_pos > newDoorsIt->stop.y_pos) {
                    newDoorsIt++;
                }
                else {
                    lastDoorsIt++;
                }

                if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
                    break;

                if(lastDoorsIt->start.y_pos >= newDoorsIt->stop.y_pos) {
                    newDoorsIt++;
                }
                else if(lastDoorsIt->stop.y_pos <= newDoorsIt->start.y_pos) {
                    lastDoorsIt++;
                }
            } while(lastDoorsIt->stop.y_pos < newDoorsIt->start.y_pos || lastDoorsIt->start.y_pos > newDoorsIt->stop.y_pos);

            if(lastDoorsIt == lastDoors.end() || newDoorsIt == newDoors.end())
                continue;
        }

        //Move new line segments to last line segments.
        lastDoors = newDoors;
        newDoors.clear();
    }
}
