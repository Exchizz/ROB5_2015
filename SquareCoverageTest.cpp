#include "World/World.h"
#include "Point/Point.h"
#include "DoorDetection/DoorDetection.h"
#include "Tree/Tree.h"
#include "DOTgraph/DOTgraph.h"
#include "SquareCoverage/SquareCoveragePlanner.h"
#include "Robot/Robot.h"


int main(){
    Image* rawImg = new Image;
    rawImg->loadImage("complete_map_project.pgm");
    rawImg->maxValue = 8191; // 2^13 - 1
    rawImg->cleanupImage();

    Point startLocation(2436,1267);
    Point endLocation(2436,1267);

    int robotSize = 4;
    int robotView = 20;



    World* Qspace1 = new World(rawImg);

    Qspace1->img->maxValue = 255;

    DoorDetection doordetection;
    auto doorways = doordetection.FindDoorways(Qspace1);

    //Draw PX (point on each site of door) and close the door
    doordetection.DrawPxAndDoors(doorways, Qspace1);

    Qspace1->img->cleanupImage();

    Qspace1->img->saveImage("DoorWorkspace.pgm");

    RoomCoverage testCoverage(startLocation, robotSize, robotView, Qspace1->img);

    testCoverage.drawViewPrintEdges();

    testCoverage.drawSquareConnections();

    Point Robot = testCoverage.pathGoToDoor(startLocation, endLocation);
    while(Robot.x != endLocation.x && Robot.y != endLocation.y);{
        Point test = Robot;
        Robot = testCoverage.getNextLocation();
        testCoverage.pathTest(test);
    }

    Qspace1->img->saveImage("coverageTest.pgm");

    // Starting at left-offloading-station
    //Robot robot(Point(624,38),Qspace1);
    //robot.scanRobotsCircumference(Point(2100,1400));
    //robot.goToPoint(Point(4742, 38));
    //robot.savePath("my_path.pgm");
    //std::cout << "Length traveled: " << robot.lengthTraveled() << std::endl;
    return 0;
}
