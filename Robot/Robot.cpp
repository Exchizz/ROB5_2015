#include "Robot.h"


Robot::Robot(void)
{
}

Robot::Robot(int Qstart_x, int Qstart_y, World* ws)
{
    this->Qstart_x = Qstart_x;
    this->Qstart_y = Qstart_y;

    workspace = new World(ws->img);
    path = new World(ws->img);
    navigationMap = new World(ws->img);
}

Robot::~Robot(void)
{
}

//Image Robot::getPath(){
//    return path->img;
//}

void Robot::savePath(){
    path->img->saveImage("robot_path.pgm");
}

void Robot::updatelength(){
    //calculate real length
    while(!pathTaken.empty()) { // as long as there are elements in the queue (pathsegments)
        pathSegment = pathTaken.front(); //take the first value
        pathTaken.pop();

        if(pathSegment.heading == straight) {
            totalLength += pathSegment.length; // if the robot went straight just add the length
        }
        else {
            totalLength += (sqrt(2 * pow(pathSegment.length, 2))); // else use pythagoras (a and b are the same for 45 degrees) to calculate the length
            //totalLength += (sqrt(pow(pathSegment.length, 2) + pow(pathSegment.length, 2))); // else use pythagoras (a and b are the same for 45 degrees) to calculate the length
        }
    }

}

int Robot::lengthTraveled(){

    std::cout << "The distance moved is " << totalLength << std::endl;
    return totalLength;
}

void Robot::goToPoint(unsigned int goal_x, unsigned int goal_y)
{
    navigationMap->img->cleanupImageRobot();
    Point start(Qstart_x, Qstart_y);
    Point stop(goal_x, goal_y);
    navigationMap->Wavefront_navigation(stop, start);
    followWavefront(navigationMap->img);
    updatelength();
}


void Robot::followWavefront(Image *map)
{
    // Pattern used for moving
    //1 check nearby pixels (first checking if going straight will decrease distance)
    //2 go to the first pixel with a lower value (the first the decreases the distance to goal)

    current_x = Qstart_x;
    current_y = Qstart_y;

    // getting the distance to goal
    int value = map->getPixel(current_x, current_y);

    // default direction is north
    direction checkThisDirection = north;


        pathSegment.length = 0;	//value used to calculate the distance traveled
        pathSegment.heading = straight;	// calculate the distance traveled (if going straight the values are added if angeled(can only be 45 degrees) pythagoras is used)
    //values are added if angeled(can only be 45 degrees) pythagoras is used)
        heading_type newHeading = straight;

    while(value != 1) {	// continue until goal is reached (goal has the value 1)
        value = navigationMap->img->getPixel(current_x, current_y);	// getting the current distance to goal
        //std::cout << value << std::endl;

        switch(checkThisDirection) // checking the surrounding pixels
        {
        case north:
            newHeading = straight;
            if(checkDirection(current_x, current_y - 1, value)) //if the distance is shorter than the current distance, move
                break;                                          //and stop searching
        case east:
            if(checkDirection(current_x + 1, current_y, value))
                break;
        case south:
            if(checkDirection(current_x, current_y + 1, value))
                break;
        case west:
            if(checkDirection(current_x - 1, current_y, value))
                break;
        case northeast:
            newHeading = angled;                                  //if it is not possible to go straight chack the angled pixels.
            if(checkDirection(current_x + 1, current_y - 1, value))
                break;
        case southeast:
            if(checkDirection(current_x + 1, current_y + 1, value))
                break;
        case southwest:
            if(checkDirection(current_x - 1, current_y + 1, value))
                break;
        case northwest:
            if(checkDirection(current_x - 1, current_y - 1, value))
                break;
        }

                if(pathSegment.heading != newHeading) { //if the heading changes (angled to straight or vice versa )
                    //push how much distance has been traveled
                    pathTaken.push(pathSegment);

                    //change last heading
                    pathSegment.heading = newHeading;

                    //reset length
                    pathSegment.length = 0;
                }
                else {
                    //increase length (if heading hasnt changed keep on going)
                    pathSegment.length++;
                    std::cout << pathSegment.length << std::endl;
                }
    }
    pathTaken.push(pathSegment); // push the last segment traveled to get to the goal
    Qstart_x = current_x;
    Qstart_y = current_y;
}

bool Robot::checkDirection(int x, int y, int value)
{
    unsigned int check_value = navigationMap->img->getPixel(x, y);
    //std::cout << "checking " << check_value << " coordiante " << x << " og " << y<< " " << value << std::endl;
    if(check_value < value && check_value != 0) {
        path->img->setPixel(current_x, current_y, 1000);
        current_x = x;
        current_y = y;
        return true;
    }
    else {
        return false;
    }
}
