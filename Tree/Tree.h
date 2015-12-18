#ifndef TREE_H
#define TREE_H

#include "../World/World.h"
#include "../Door/Door.h"

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
};


#endif // TREE_H
