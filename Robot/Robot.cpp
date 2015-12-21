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

    offloadingMap = new World(ws->img);
    offloadingMap->Wavefront_offloading();
}

Robot::~Robot(void)
{
}

//Image Robot::getPath(){
//    return path->img;
//}

void Robot::savePath(std::string filename){
    path->img->saveImage(filename);
}

double Robot::lengthTraveled(){
    std::cout << Qstart_x << " " << Qstart_y << std::endl;
    return totalLength;
}

void Robot::goToPoint(Point stop)
{
    navigationMap->img->cleanupImageRobot();
    Point start(Qstart_x, Qstart_y);
    if(stop.x != currentMovingToPosition.x && stop.y != currentMovingToPosition.y)
        currentMovingToPosition = stop;
    navigationMap->Wavefront_navigation(stop, start);
    followWavefront(navigationMap);
    //updatelength();
}


bool Robot::scanRobotsCircumference(Point pose){
    if(returningHome)
        return false;
    for(signed int x = -5; x < 5; ++x){
        for(signed int y = -5; y < 5; ++y){
            if(workspace->img->getPixel(pose.x + x, pose.y + y) == CUP){
                workspace->img->setPixel(pose.x + x, pose.y + y,workspace->img->maxValue);
                path->img->setPixel(pose.x + x, pose.y + y,workspace->img->maxValue);
                //std::cout << "Cup picked up at: " << pose.x + x << "," << pose.y + y << "from position" << pose.x << "," << pose.y << std::endl;
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

    while(value != 1) {	// continue until goal is reached (goal has the value 1)
        value = navigationMap->img->getPixel(current_x, current_y);	// getting the current distance to goal

        if(scanRobotsCircumference(Point(current_x, current_y))){
            std::cout << "going back" << std::endl;
            returningHome = true;
            Point currentPose = Point(current_x, current_y);
            cupsPickedUp = 0;
            followWavefront(offloadingMap);
            goToPoint(currentPose);
            navigationMap->img->cleanupImageRobot();
            navigationMap->Wavefront_navigation(currentMovingToPosition, currentPose);
            noPath = true;
            returningHome = false;
        }

        switch(checkThisDirection) // checking the surrounding pixels
        {
        case north:
            if(checkDirection(current_x, current_y - 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case east:
            if(checkDirection(current_x + 1, current_y, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case south:
            if(checkDirection(current_x, current_y + 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case west:
            if(checkDirection(current_x - 1, current_y, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength++;
                break;                                          //and stop searching
            }
        case northeast:                            //if it is not possible to go straight chack the angled pixels.
            if(checkDirection(current_x + 1, current_y - 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case southeast:
            if(checkDirection(current_x + 1, current_y + 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case southwest:
            if(checkDirection(current_x - 1, current_y + 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        case northwest:
            if(checkDirection(current_x - 1, current_y - 1, value)){ //if the distance is shorter than the current distance, move
                noPath = false;
                totalLength += sqrt(2);
                break;                                          //and stop searching
            }
        }

        if(noPath){
            std::cout << "No Path could be found" << std::endl;
            savePath("Robot_Nopath.pgm");
            exit(0);
        }
    }
    Qstart_x = current_x;
    Qstart_y = current_y;
    noPath = true;
    std::cout << totalLength << std::endl;
}

bool Robot::checkDirection(int x, int y, int value)
{
    unsigned int check_value = navigationMap->img->getPixel(x, y);
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
