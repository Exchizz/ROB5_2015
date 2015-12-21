#include "Robot/Robot.h"


int main(){
    Image* rawImg = new Image;
    rawImg->loadImage("complete_map_project_cuptest.pgm");
    rawImg->maxValue = 8192; // 2^13 - 1
    rawImg->cleanupImage();

    World* Qspace1 = new World(rawImg);
//    Qspace1->WallExpansion();
    Qspace1->img->saveImage("cup_qspace.pgm");

    // Starting at left-offloading-station
    Robot robot(Point(2393,1303),Qspace1);
    robot.goToPoint(Point(2390, 1303));
    robot.savePath("my_path.pgm");
    robot.workspace->img->saveImage("cup_workspace.pgm");
    std::cout << "Length traveled: " << robot.lengthTraveled() << std::endl;
    return 0;
}
