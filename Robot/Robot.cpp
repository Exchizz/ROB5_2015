#include "Robot.h"

#define OFFLOADING_1 Point()


Robot::Robot(void)
{
}

Robot::Robot(Point startPoint, World* ws)
{

    this->Qstart_x = startPoint.x;
    this->Qstart_y = startPoint.y;

    workspace = new World(ws->img);
    path = new World(ws->img);
    navigationMap = new World(ws->img);
    navigationMap->img->cleanupImageRobot();

    offloadingMap = new World(ws->img);
    offloadingMap->img->cleanupImageRobot();
    offloadingMap->Wavefront_offloading();
}

Robot::~Robot(void)
{
}

void Robot::savePath(std::string filename){
    path->img->saveImage(filename);
}

double Robot::lengthTraveled(){
    return totalLength;
}

void Robot::goToPoint(Point stop)
{
	if(stop.x == current_x && stop.y == current_y){
		return;
	}
    navigationMap->img->cleanupImageRobot();
    Point start(Qstart_x, Qstart_y);
    if(!returningHome) // not on the way back from offloading station.
        currentMovingToPosition = stop;
    navigationMap->Wavefront_navigation(stop, start);
    followWavefront(navigationMap);
}


bool Robot::scanRobotsCircumference(Point pose){
    if(returningHome)
        return false;
    for(signed int x = -5; x < 5; ++x){
        for(signed int y = -5; y < 5; ++y){
            if(workspace->img->getPixel(pose.x + x, pose.y + y) == CUP){
                workspace->img->setPixel(pose.x + x, pose.y + y,workspace->img->maxValue);
                path->img->setPixel(pose.x + x, pose.y + y,workspace->img->maxValue);
                std::cout << "Cup picked up at: " << pose.x + x << "," << pose.y + y << "from position" << pose.x << "," << pose.y << " cups picked up: " << cupsPickedUp << std::endl;
                cupsPickedUp++;
                if(cupsPickedUp == 20){
                    return true;
                }
            }
        }
    }
    return false;
}



void Robot::followWavefront(World *map)
{
    // Pattern used for moving
    //1 check nearby pixels (first checking if going straight will decrease distance)
    //2 go to the first pixel with a lower value (the first the decreases the distance to goal)

    current_x = Qstart_x;
    current_y = Qstart_y;
    // getting the distance to goal
    int value = map->img->getPixel(current_x, current_y);

    // default direction is north
    direction checkThisDirection = north;

    bool noPath = true;
    value = map->img->getPixel(current_x, current_y);	// getting the current distance to goal

    while(value != 1) {	// continue until goal is reached (goal has the value 1)
        if(scanRobotsCircumference(Point(current_x, current_y))){
            std::cout << "Full of cups" << std::endl;
            returningHome = true;
            Qstart_x = current_x; // current_x = Qstart_x at start of follow wayfront.
            Qstart_y = current_y;
            Point currentPose = Point(current_x, current_y);
            cupsPickedUp = 0;
            followWavefront(offloadingMap);
            goToPoint(currentPose);
            map->img->cleanupImageRobot();
            map->Wavefront_navigation(currentMovingToPosition, currentPose);
            map->img->saveImage("continueMap.pgm");
            returningHome = false;
        }

        noPath = true;

        switch(checkThisDirection) // checking tvalue1he surrounding pixels
        {
        case north:
            if(checkDirection(current_x, current_y - 1, value, map)){ //if the distance is shorter than the current distance, move
                //std::cout << "north" << std::endl;
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case east:
            if(checkDirection(current_x + 1, current_y, value, map)){ //if the distance is shorter than the current distance, move
                //std::cout << "east" << std::endl;
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case south:
            if(checkDirection(current_x, current_y + 1, value, map)){ //if the distance is shorter than the current distance, move
                //std::cout << "south" << std::endl;
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case west:
            if(checkDirection(current_x - 1, current_y, value, map)){ //if the distance is shorter than the current distance, move
                //std::cout << "west" << std::endl;
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case northeast:                            //if it is not possible to go straight chack the angled pixels.
            if(checkDirection(current_x + 1, current_y - 1, value, map)){ //if the distance is shorter than the current distance, move
//                std::cout << "northeast" << std::endl;
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case southeast:
            if(checkDirection(current_x + 1, current_y + 1, value, map)){ //if the distance is shorter than the current distance, move
//                std::cout << "southeast" << std::endl;
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case southwest:
            if(checkDirection(current_x - 1, current_y + 1, value, map)){ //if the distance is shorter than the current distance, move
//                std::cout << "southwest" << std::endl;
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case northwest:
            if(checkDirection(current_x - 1, current_y - 1, value, map)){ //if the distance is shorter than the current distance, move
//                std::cout << "northwest" << std::endl;
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        }

        if(noPath){
            std::cout << "No Path could be found" << current_x << " " << current_y << std::endl;
            savePath("Robot_Nopath.pgm");
            exit(0);
        }
        value = map->img->getPixel(current_x, current_y);	// getting the current distance to goal

    }
    Qstart_x = current_x;
    Qstart_y = current_y;
    noPath = true;
    //std::cout << "traveled " << totalLength << " currently at " << current_x << " " << current_y << std::endl;
}

bool Robot::checkDirection(int x, int y, int value, World* map)
{
    unsigned int check_value = map->img->getPixel(x, y);
    //std::cout << "checking " << check_value << " coordiante " << x << " og " << y<< " " << value << std::endl;
    if(check_value < value && check_value != 0) {
        path->img->setPixel(current_x, current_y, 4000);
        current_x = x;
        current_y = y;
        return true;
    }
    else {
        return false;
    }
}
