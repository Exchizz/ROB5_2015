#include "Tree.h"

Tree::Tree()
{

}

Tree::Tree(World* world)
{
    this->world = world;
}


std::vector<Door> Tree::door_hitpoint_merge(std::vector<Door> & doorways, std::vector<Point> &wavefront_hitpoint_px){
    std::vector<Door> output;
    for (auto px : wavefront_hitpoint_px){
        for(int i = 0; i < doorways.size(); ++i){
            if(px.x_pos == doorways[i].px1.x_pos && px.y_pos == doorways[i].px1.y_pos){
                 doorways[i].px1_visited = true;
                 output.push_back(doorways[i]);
            }
            if(px.x_pos == doorways[i].px2.x_pos && px.y_pos == doorways[i].px1.y_pos){
                 doorways[i].px2_visited = true;
                 output.push_back(doorways[i]);
            }
        }
    }

    return output;
}

std::vector<Node> Tree::Tree_generator(Point start, std::vector<Door> &doorways){

    std::queue<Door> RoomToBeServed;
    bool runonce = true;
    int i = 4;

    while(!RoomToBeServed.empty() || runonce){
        Door startFrom = RoomToBeServed.front();
        RoomToBeServed.pop();

        Point startPoint;

        if(startFrom.px1_visited){
            startPoint = startFrom.px2; // Start from opposite site of door
        } else if(startFrom.px2_visited){
            startPoint = startFrom.px1;
        } else {
            std::cout << "Room with no doors" << std::endl;
        }

        if(runonce){
            startPoint = start;
            runonce = false;
        }

        std::cout << "startign at (x,y): (" << startPoint.x_pos << "," << startPoint.y_pos << ")" << std::endl;

        // find doors from offload station
        auto hit_px_doors = world->Wavefront_DoorScanner(startPoint, 127, 126);


        // Merge doors with hitspoints
        auto doorways_visisted = door_hitpoint_merge(doorways, hit_px_doors);

        //Run through every door and mark hitpoint
        for (auto door : doorways_visisted){
            if(door.px1_visited){
                world->img->setPixel(door.px1.x_pos, door.px1.y_pos, 2887);
                RoomToBeServed.push(door);

            } else if(door.px2_visited){
                world->img->setPixel(door.px2.x_pos, door.px2.y_pos, 2887);
                RoomToBeServed.push(door);

            }

 //           std::cout << "qeue size: " << RoomToBeServed.size() << std::endl;
        }

        if(--i == 0){
            std::vector<Node> output;
            return output;
        }

    }

    std::vector<Node> output;
    return output;
}
