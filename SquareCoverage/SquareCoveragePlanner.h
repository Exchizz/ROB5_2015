#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <list>
#include <queue>
#include "../Image/Image.h"
#include "../Point/Point.h"

#define OVER	0
#define RIGHT	1
#define UNDER	2
#define LEFT	3
#define OVERRIGHT	4
#define UNDERRIGHT	5
#define UNDERLEFT	6
#define	OVERLEFT	7
#define NODIRECTION	8

#define CLEAR			255
#define WALL			0
#define VIEWPRINTEDGE	125
#define FOOTPRINTEDGE	100
#define VIEWPRINTFILL	200
#define FOOTPRINTFILL	175
#define SQUARECONNECT	225
#define ROBOTPATH		25

class RoomCoverage {
public:
    RoomCoverage(Point roomStart, int robotFootprint, int robotViewPrint, Image *ws);
	~RoomCoverage();
		
private:
	struct square {
        square(Point center);
        Point center;
		int sideLength;
		std::array<square*, 8> adjacentSquares;
		bool goodSquare;
		bool cleared;

		bool operator==(const square& compare);
	};

    Image* ws;

	double viewPrintSize;
	double footPrintSize;

	square* exitSquare;
    Point exitPoint;
    Point entryPoint;
	bool covering;

	square *currentSquare;
	std::array<int, 4> directionVectorWeight;
	std::list<square*> squaresInTheRoom;

	int calculateSquareLength(int radius);
	
	/*Making workspace for room*/
    void fillRoomWithSquares(Point startingPoint);
    Point findFirstSquare(Point startingPoint);
    Point findSquarePlacement(Point startingPoint, int direction);
    Point findNeighborOnPath(Point startingPoint, Point endPoint, int direction);
    bool scanViewPrintEdge(Point scanPoint, int direction);
    bool scanFootPrintEdge(Point scanPoint, int direction);
    bool checkForExpansion(Point startingPoint, int direction);
    bool checkForClearPath(Point startingPoint, Point endPoint, int direction);

	void findOrthogonalNeighbors(square* homesquare);
	void findDiagonalNeighbors(square *homeSquare);


	/*path planning*/
	int getSquareWeight(square*);
	int getClosestSquareDirection(square*);
    square* findNearestSquare(Point);
	square* findNextSquare();
	void findOptimalRoute();

	void cleanWorkspace();


	/*for testing*/
    void drawPath(Point startingPoint, Point endPoint);

public:
    Point getNextLocation();			//find the next square to move to.

    Point pathCoverRoom(Point entry, Point exit);
    Point pathGoToDoor(Point entry, Point exit);
    void pathTest(Point nextLocation);

	void drawFootPrintEdges();
	void drawViewPrintEdges();
	void drawFootPrintFill();
	void drawViewPrintFill();
	void drawSquareConnections();

	bool roomCleared();

	//testing
	int distanceTravelled;
	
};
