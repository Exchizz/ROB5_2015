#pragma once

#include <iostream>
#include <queue>
#include <math.h>
#include "../Image/Image.h"
#include "../World/World.h"
#include "../Point/Point.h"

enum heading_type {straight, angled};

class Robot
{
private:
    enum direction {north, east, south, west, northeast, southeast, southwest, northwest};
public:
    World* workspace;
    //Image* qspaec;
    World* path;
    World* navigationMap;
    World* offloadingMap;

    unsigned int Qstart_x, Qstart_y;
    unsigned int current_x, current_y;
    double totalLength = 0;
    bool returningHome = false;

    unsigned int cupsPickedUp = 0;

    bool checkDirection(int x, int y, int value);
    void followWavefront(World* map);

    Robot(void);
    Robot(Point , World *ws);

    bool scanRobotsCircumference(Point pose);

    void goToPoint(Point);
    Point currentMovingToPosition;
    Image getPath();
    void savePath(std::string);
    void updatelength();
    double lengthTraveled();
    ~Robot(void);

};
