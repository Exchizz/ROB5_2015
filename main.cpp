#include "Robot/Robot.h"


int main(){
	Image* rawImg = new Image;
	rawImg->loadImage("complete_map_project.pgm");
	rawImg->maxValue = 8192; // 2^13 - 1
	rawImg->cleanupImage();

	World* Qspace1 = new World(rawImg);
	Qspace1->WallExpansion();

	// Starting at left-offloading-station
	Robot robot(Point(624,38),Qspace1);
	robot.goToPoint(Point(4742, 38));
	robot.savePath("my_path.pgm");
	std::cout << "Length traveled: " << robot.lengthTraveled() << std::endl;
	return 0;
}
