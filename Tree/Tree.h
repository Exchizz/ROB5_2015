#ifndef TREE_H
#define TREE_H

#include "../World/World.h"
#include "../Door/Door.h"

class EntryPoint{
	Point Enter;
	Point Leave;
};

class Node {

public:
    std::vector<Node> adjacent;
};


class Tree {
    World* world;


public:
    Tree();

    Tree(World* world);
    std::vector<Door> door_hitpoint_merge(std::vector<Door> &, std::vector<Point> &);

    std::vector<Door> Tree_generator(Point &start, std::vector<Door> &doorways);
    std::vector<Point> GenerateNavigationList(Door door);
    //std::vector<Door> GenerateNavigationList(Door door);
    Door AddToList(std::vector<Point> &plannedPath, Door &parent);
    //Door AddToList(std::vector<Door> &doorList, Door &parent);

};


#endif // TREE_H
