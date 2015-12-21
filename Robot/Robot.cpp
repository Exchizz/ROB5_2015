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

double Robot::lengthTraveled(){
	return totalLength;
}

void Robot::goToPoint(Point stop)
{
	navigationMap->img->cleanupImageRobot();
	Point start(Qstart_x, Qstart_y);
	currentMovingToPosition = stop;
	navigationMap->Wavefront_navigation(stop, start);
	followWavefront(navigationMap);
	updatelength();
}


bool Robot::scanRobotsCircumference(Point pose){
	for(signed int x = -5; x < 5; ++x){
		for(signed int y = -5; y < 5; ++y){
			if(workspace->img->getPixel(pose.x + x, pose.y + y) == CUP){
				std::cout << "Cup picked up" << std::endl;
				cupsPickedUp++;
				if(cupsPickedUp == 20){
					return true;
				}

			}
			path->img->setPixel(pose.x + x, pose.y + y, 1000);

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


	pathSegment.length = 0;	//value used to calculate the distance traveled
	pathSegment.heading = straight;	// calculate the distance traveled (if going straight the values are added if angeled(can only be 45 degrees) pythagoras is used)
	//values are added if angeled(can only be 45 degrees) pythagoras is used)
	heading_type newHeading = straight;
	bool noPath = true;

	while(value != 1) {	// continue until goal is reached (goal has the value 1)
		value = navigationMap->img->getPixel(current_x, current_y);	// getting the current distance to goal

		if(scanRobotsCircumference(Point(current_x, current_y))){
			std::cout << "Moving to offloading station" << std::endl;
			Point currentPose = Point(current_x, current_y);

			cupsPickedUp = 0;
			std::cout << "Goto: offloading " << std::endl;
			followWavefront(offloadingMap);
			std::cout << "Goto: current pose " << std::endl;
			goToPoint(currentPose);
			navigationMap->img->cleanupImageRobot();
			navigationMap->Wavefront_navigation(currentMovingToPosition, currentPose);
			followWavefront(navigationMap);
		}

		switch(checkThisDirection) // checking the surrounding pixels
		{
		case north:
			newHeading = straight;
			if(checkDirection(current_x, current_y - 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case east:
			if(checkDirection(current_x + 1, current_y, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case south:
			if(checkDirection(current_x, current_y + 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case west:
			if(checkDirection(current_x - 1, current_y, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case northeast:
			newHeading = angled;                                  //if it is not possible to go straight chack the angled pixels.
			if(checkDirection(current_x + 1, current_y - 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case southeast:
			if(checkDirection(current_x + 1, current_y + 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case southwest:
			if(checkDirection(current_x - 1, current_y + 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		case northwest:
			if(checkDirection(current_x - 1, current_y - 1, value)){ //if the distance is shorter than the current distance, move
				noPath = false;
				break;                                          //and stop searching
			}
		}

		if(noPath){
			std::cout << "No Path could be found" << std::endl;
			savePath("Robot_Nopath.pgm");
			exit(0);
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
		}
	}
	pathTaken.push(pathSegment); // push the last segment traveled to get to the goal
	Qstart_x = current_x;
	Qstart_y = current_y;
	noPath = true;
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
