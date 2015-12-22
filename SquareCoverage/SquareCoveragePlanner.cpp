#include "SquareCoveragePlanner.h"



RoomCoverage::RoomCoverage(Point roomStart, int robotFootprint, int robotViewPrint, Image *workspace)
{
    ws = workspace;

	//calculate square length
	viewPrintSize = calculateSquareLength(robotViewPrint);
	footPrintSize = robotFootprint * 2;

	if(viewPrintSize < footPrintSize) 
		viewPrintSize = footPrintSize;

	//Find start location for square wavefront.
	//is it possible to start from the door location?
	//Fill room with squares using wavefront
    //Connect the adjacent squares using Pointers
	fillRoomWithSquares(roomStart);
	
	//complete
	directionVectorWeight[OVER]	 = 2;
	directionVectorWeight[RIGHT] = 3;
	directionVectorWeight[UNDER] = 1;
	directionVectorWeight[LEFT]  = 0; 

	distanceTravelled = 0;
}

RoomCoverage::~RoomCoverage()
{
	for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
		delete *IT;
	}
}

void RoomCoverage::fillRoomWithSquares(Point startingPoint)
{
    square *aSquarePointer, *aNewSquarePointer;
    Point centerPoint;

//find locaion of the first square.
	std::cout << "making first square" << std::endl;
    centerPoint = findFirstSquare(startingPoint);

//make the first square.
	std::queue<square*> goodSquares, badSquares, reallyBadSquares;
    aNewSquarePointer = new square(centerPoint);
    goodSquares.push(aNewSquarePointer);
    squaresInTheRoom.push_back(aNewSquarePointer);
	
	//give location of the first square to the class for later track and tracing.
    currentSquare = aNewSquarePointer;

//make adjacent squares (good squares).
	std::cout << "making adjacent squares" << std::endl;
	while(!goodSquares.empty()) {
		//grab first square
        aSquarePointer = goodSquares.front();
		goodSquares.pop();

	//check over
        if(aSquarePointer->adjacentSquares[OVER] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, OVER);
                if(checkForClearPath(aSquarePointer->center, centerPoint, OVER)) {
                if(centerPoint.y != aSquarePointer->center.y) {
                    aNewSquarePointer = new square(centerPoint);
                    squaresInTheRoom.push_back(aNewSquarePointer);
                    findOrthogonalNeighbors(aNewSquarePointer);
                    if(static_cast<int>(aSquarePointer->center.y - centerPoint.y) < viewPrintSize) {
                        aNewSquarePointer->goodSquare = false;
                        badSquares.push(aNewSquarePointer);
					}
					else {
                        goodSquares.push(aNewSquarePointer);
					}
				}
			}
		}

	//check under
        if(aSquarePointer->adjacentSquares[UNDER] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, UNDER);
                if(checkForClearPath(aSquarePointer->center, centerPoint, UNDER)) {
                if(centerPoint.y != aSquarePointer->center.y) {
                    aNewSquarePointer = new square(centerPoint);
                    squaresInTheRoom.push_back(aNewSquarePointer);
                    findOrthogonalNeighbors(aNewSquarePointer);
                    if(static_cast<int>(centerPoint.y - aSquarePointer->center.y) < viewPrintSize) {
                        aNewSquarePointer->goodSquare = false;
                        badSquares.push(aNewSquarePointer);
					}
					else {
                        goodSquares.push(aNewSquarePointer);
					}
				}
			}
		}

	//check right
        if(aSquarePointer->adjacentSquares[RIGHT] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, RIGHT);
            if(checkForClearPath(aSquarePointer->center, centerPoint, RIGHT)) {
                if(centerPoint.x != aSquarePointer->center.x) {
                    aNewSquarePointer = new square(centerPoint);
                    squaresInTheRoom.push_back(aNewSquarePointer);
                    findOrthogonalNeighbors(aNewSquarePointer);
                    if(static_cast<int>(centerPoint.x - aSquarePointer->center.x) < viewPrintSize) {
//						std::cout << "Pushing bad square" << std::endl;
                        aNewSquarePointer->goodSquare = false;
                        badSquares.push(aNewSquarePointer);
					}
					else {
//						std::cout << "Pushing good square" << std::endl;
                        goodSquares.push(aNewSquarePointer);
					}
				}
			}
		}
	
	//check left
        if(aSquarePointer->adjacentSquares[LEFT] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, LEFT);
            if(checkForClearPath(aSquarePointer->center, centerPoint, LEFT)) {
                if(centerPoint.x != aSquarePointer->center.x) {
                    aNewSquarePointer = new square(centerPoint);
                    squaresInTheRoom.push_back(aNewSquarePointer);
                    findOrthogonalNeighbors(aNewSquarePointer);
                    if(static_cast<int>(aSquarePointer->center.x - centerPoint.x) < viewPrintSize) {
                        aNewSquarePointer->goodSquare = false;
                        badSquares.push(aNewSquarePointer);
					}
					else {
                        goodSquares.push(aNewSquarePointer);
					}
				}
			}
		}
	}

	std::cout << "Making diagonal neighbors" << std::endl;
//make diagonal neighbor connections
	for(auto squaresInTheRoomIt = squaresInTheRoom.begin(); squaresInTheRoomIt != squaresInTheRoom.end(); squaresInTheRoomIt++) {
		findDiagonalNeighbors(*squaresInTheRoomIt);
	}

//handle bad squares
	std::cout << "Making bad squares" << std::endl;
	while(!badSquares.empty()) {
				//grab first square
        aSquarePointer = badSquares.front();
		badSquares.pop();

        Point neighborPoint;
		bool doPlacement;

	////check under
        if(aSquarePointer->adjacentSquares[UNDER] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, UNDER);
            neighborPoint.x = centerPoint.x;
            neighborPoint.y = centerPoint.y + 1;
            neighborPoint = findNeighborOnPath(aSquarePointer->center, neighborPoint, UNDER);
            if(neighborPoint.x == centerPoint.x && neighborPoint.y == centerPoint.y + 1) {
                if(centerPoint.y - aSquarePointer->center.y == viewPrintSize) {
					doPlacement = true;
				}
				else{
					doPlacement = false;
				}
			}
			else {
                centerPoint = neighborPoint;
				doPlacement = true;
			}
			if(doPlacement) {
                if(checkForClearPath(aSquarePointer->center, centerPoint, UNDER)) {
                    if(centerPoint.y != aSquarePointer->center.y) {
                        aNewSquarePointer = new square(centerPoint);
                        aNewSquarePointer->goodSquare = false;
                        squaresInTheRoom.push_back(aNewSquarePointer);
                        findOrthogonalNeighbors(aNewSquarePointer);
                        if(getSquareWeight(aNewSquarePointer) < 2) {
                            reallyBadSquares.push(aNewSquarePointer);
                            //badSquares.push(aNewSquarePointer);
						}
					}
				}
			}
		}

//	//check right
        if(aSquarePointer->adjacentSquares[RIGHT] == nullptr) {
//			std::cout << "checking square: (" << aSquarePointer->center.x << ", " << aSquarePointer->center.y << ")" << std::endl;
            centerPoint = findSquarePlacement(aSquarePointer->center, RIGHT);
            neighborPoint.x = centerPoint.x + 1;
            neighborPoint.y = centerPoint.y;
            neighborPoint = findNeighborOnPath(aSquarePointer->center, neighborPoint, RIGHT);
            if(neighborPoint.x == centerPoint.x + 1 && neighborPoint.y == centerPoint.y) {
//				std::cout << "There is no neighbor" << std::endl;
                if(centerPoint.x - aSquarePointer->center.x == viewPrintSize) {
					doPlacement = true;
				}
				else{
					doPlacement = false;
				}
			}
			else {
//				std::cout << "placing new neighbor" << std::endl;
                centerPoint = neighborPoint;
				doPlacement = true;
			}
			if(doPlacement) {
                if(checkForClearPath(aSquarePointer->center, centerPoint, RIGHT)) {
                    if(centerPoint.x != aSquarePointer->center.x) {
                        aNewSquarePointer = new square(centerPoint);
                        aNewSquarePointer->goodSquare = false;
                        squaresInTheRoom.push_back(aNewSquarePointer);
                        findOrthogonalNeighbors(aNewSquarePointer);
                        if(getSquareWeight(aNewSquarePointer) < 2) {
                            reallyBadSquares.push(aNewSquarePointer);
                            //badSquares.push(aNewSquarePointer);
						}
					}
				}
			}
		}
	
	//check left
        if(aSquarePointer->adjacentSquares[LEFT] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, LEFT);
            neighborPoint.x = centerPoint.x - 1;
            neighborPoint.y = centerPoint.y;
            neighborPoint = findNeighborOnPath(aSquarePointer->center, neighborPoint, LEFT);
            if(neighborPoint.x == centerPoint.x - 1 && neighborPoint.y == centerPoint.y) {
                if(aSquarePointer->center.x - centerPoint.x == viewPrintSize) {
					doPlacement = true;
				}
				else{
					doPlacement = false;
				}
			}
			else {
                centerPoint = neighborPoint;
				doPlacement = true;
			}
			if(doPlacement) {
                if(checkForClearPath(aSquarePointer->center, centerPoint, LEFT)) {
                    if(centerPoint.x != aSquarePointer->center.x) {
                        aNewSquarePointer = new square(centerPoint);
                        aNewSquarePointer->goodSquare = false;
                        squaresInTheRoom.push_back(aNewSquarePointer);
                        findOrthogonalNeighbors(aNewSquarePointer);
                        if(getSquareWeight(aNewSquarePointer) < 2) {
                            reallyBadSquares.push(aNewSquarePointer);
                            //badSquares.push(aNewSquarePointer);
						}
					}
				}
			}
		}

	//check over
        if(aSquarePointer->adjacentSquares[OVER] == nullptr) {
            centerPoint = findSquarePlacement(aSquarePointer->center, OVER);
            neighborPoint.x = centerPoint.x;
            neighborPoint.y = centerPoint.y - 1;
            neighborPoint = findNeighborOnPath(aSquarePointer->center, centerPoint, OVER);
            if(neighborPoint.x == centerPoint.x && neighborPoint.y == centerPoint.y - 1) {
                if(aSquarePointer->center.y - centerPoint.y == viewPrintSize) {
					doPlacement = true;
				}
				else{
					doPlacement = false;
				}
			}
			else {
                centerPoint = neighborPoint;
				doPlacement = true;
			}
			if(doPlacement) {
                if(checkForClearPath(aSquarePointer->center, centerPoint, OVER)) {
                    if(centerPoint.y != aSquarePointer->center.y) {
                        aNewSquarePointer = new square(centerPoint);
                        aNewSquarePointer->goodSquare = false;
                        squaresInTheRoom.push_back(aNewSquarePointer);
                        findOrthogonalNeighbors(aNewSquarePointer);
                        if(getSquareWeight(aNewSquarePointer) < 2) {
                            reallyBadSquares.push(aNewSquarePointer);
                            //badSquares.push(aNewSquarePointer);
						}
					}
				}
			}
		}
	}
}

Point RoomCoverage::findNeighborOnPath(Point startingPoint, Point endPoint, int direction)
{
    Point nearestNeighbor = endPoint;

	if(direction == OVER) {
		for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
            if( ((*IT)->center.y < startingPoint.y) && ((*IT)->center.y > endPoint.y) ) {
                if( ((*IT)->center.x >= startingPoint.x - viewPrintSize) && ((*IT)->center.x <= startingPoint.x + viewPrintSize)) {
					if((*IT)->center.y > nearestNeighbor.y) {
						nearestNeighbor.y = (*IT)->center.y;
					}
				}
			}
		}
	}
	else if(direction == UNDER) {
		for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
            if( ((*IT)->center.y > startingPoint.y) && ((*IT)->center.y < endPoint.y) ) {
                if( ((*IT)->center.x >= startingPoint.x - viewPrintSize) && ((*IT)->center.x <= startingPoint.x + viewPrintSize)) {
					if((*IT)->center.y < nearestNeighbor.y) {
						nearestNeighbor.y = (*IT)->center.y;
					}
				}
			}
		}
	}
	else if(direction == RIGHT) {
		for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
            if( ((*IT)->center.x > startingPoint.x) && ((*IT)->center.x < endPoint.x) ) {
                if( ((*IT)->center.y >= startingPoint.y - viewPrintSize) && ((*IT)->center.y <= startingPoint.y + viewPrintSize)) {
					if((*IT)->center.x < nearestNeighbor.x) {
//						std::cout << "I found a neighbor" << std::endl;
						nearestNeighbor.x = (*IT)->center.x;
					}
				}
			}
		}
	}
	else if(direction == LEFT) {
		for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
            if( ((*IT)->center.x < startingPoint.x) && ((*IT)->center.x > endPoint.x) ) {
                if( ((*IT)->center.y >= startingPoint.y - viewPrintSize) && ((*IT)->center.y <= startingPoint.y + viewPrintSize)) {
					if((*IT)->center.x > nearestNeighbor.x) {
						nearestNeighbor.x = (*IT)->center.x;
					}
				}
			}
		}
	}

	return nearestNeighbor;
}


bool RoomCoverage::checkForClearPath(Point startingPoint, Point endPoint, int direction)
{
	if(direction == OVER) {
        for(double x = startingPoint.x - footPrintSize/2; x <= startingPoint.x + footPrintSize/2; x++) {
            for(double y = startingPoint.y; y >= endPoint.y; y--) {
                if(ws->getPixel(static_cast<int>(x), static_cast<int>(y)) == WALL) {
					return false;
				}
			}
		}
	}	
	else if(direction == UNDER) {
        for(double x = startingPoint.x - footPrintSize/2; x <= startingPoint.x + footPrintSize/2; x++) {
            for(double y = startingPoint.y; y <= endPoint.y; y++) {
                if(ws->getPixel(static_cast<int>(x), static_cast<int>(y)) == WALL) {
					return false;
				}
			}
		}
	}
	else if(direction == RIGHT) {
        for(double y = startingPoint.y - footPrintSize/2; y <= startingPoint.y + footPrintSize/2; y++) {
            for(double x = startingPoint.x; x <= endPoint.x; x++) {
                if(ws->getPixel(static_cast<int>(x), static_cast<int>(y)) == WALL) {
					return false;
				}
			}
		}
	}
	else if(direction == LEFT) {
        for(double y = startingPoint.y - footPrintSize/2; y <= startingPoint.y + footPrintSize/2; y++) {
            for(double x = startingPoint.x; x >= endPoint.x; x--) {
                if(ws->getPixel(static_cast<int>(x), static_cast<int>(y)) == WALL) {
					return false;
				}
			}
		}
	}

	return true;
}

bool RoomCoverage::checkForExpansion(Point startingPoint, int direction)
{
	if(direction == OVER) {
        for(double y = startingPoint.y; y >= startingPoint.y - viewPrintSize/2; y--) {
            if(ws->getPixel(static_cast<int>(startingPoint.x), static_cast<int>(y)) == WALL) {
				return false;
			}
		}
	}
	else if(direction == UNDER) {
        for(double y = startingPoint.y; y <= startingPoint.y + viewPrintSize/2; y++) {
            if(ws->getPixel(static_cast<int>(startingPoint.x), static_cast<int>(y)) == WALL) {
				return false;
			}
		}
	}
	else if(direction == LEFT) {
        for(double x = startingPoint.x; x >= startingPoint.x - viewPrintSize/2; x--) {
            if(ws->getPixel(static_cast<int>(x), static_cast<int>(startingPoint.y)) == WALL) {
				return false;
			}
		}
	}
	else if(direction == RIGHT) {
        for(double x = startingPoint.x; x <= startingPoint.x + viewPrintSize/2; x++) {
            if(ws->getPixel(static_cast<int>(x), static_cast<int>(startingPoint.y)) == WALL) {
				return false;
			}
		}
	}
	else {
		std::cout << "checkForExpansion: Wrong direction" << std::endl;
		return false;
	}
	
	return true;
}

Point RoomCoverage::findFirstSquare(Point startingPoint)
{
	int doorPlacement;

    std::cout << "starting Point: (" << startingPoint.x << ", " << startingPoint.y << ")" << std::endl;
		
    if(ws->getPixel(static_cast<int>(startingPoint.x + 1), static_cast<int>(startingPoint.y)) == WALL) {
		doorPlacement = RIGHT;
//		std::cout << "The door is on the right" << std::endl;
	}
    else if(ws->getPixel(static_cast<int>(startingPoint.x - 1), static_cast<int>(startingPoint.y)) == WALL) {
		doorPlacement = LEFT;
		std::cout << "The door is on the left" << std::endl;
	}
    else if(ws->getPixel(static_cast<int>(startingPoint.x), static_cast<int>(startingPoint.y + 1)) == WALL) {
		doorPlacement = UNDER;
//		std::cout << "The door is underneath" << std::endl;
	}
    else if(ws->getPixel(static_cast<int>(startingPoint.x), static_cast<int>(startingPoint.y - 1)) == WALL) {
		doorPlacement = OVER;
//		std::cout << "The door is over" << std::endl;
	}
	else {
		std::cout << "There was no door.." << std::endl;
        return Point(0,0);
	}

    Point newCenter = startingPoint;
    Point tempCenter;

	if(doorPlacement == OVER) {
		newCenter.y += footPrintSize/2;
		tempCenter = findSquarePlacement(newCenter, UNDER);

		if((tempCenter.y - newCenter.y) > (viewPrintSize - footPrintSize) / 2 ) {
			newCenter.y += (viewPrintSize - footPrintSize) / 2;
		}
		else {
			newCenter = tempCenter;
		}
	}
	else if(doorPlacement == UNDER) {
		newCenter.y -= footPrintSize/2;
		tempCenter = findSquarePlacement(newCenter, OVER);

		if((newCenter.y - tempCenter.y) > (viewPrintSize - footPrintSize) / 2 ) {
			newCenter.y -= (viewPrintSize - footPrintSize) / 2;
		}
		else {
			newCenter = tempCenter;
		}
	}
	else if(doorPlacement == RIGHT) {
		newCenter.x -= footPrintSize/2;
		tempCenter = findSquarePlacement(newCenter, LEFT);

		if((newCenter.x - tempCenter.x) > (viewPrintSize - footPrintSize) / 2 ) {
			newCenter.x -= (viewPrintSize - footPrintSize) / 2;
		}
		else {
			newCenter = tempCenter;
		}
	}
	else if(doorPlacement == LEFT) {
		newCenter.x += footPrintSize/2;
		tempCenter = findSquarePlacement(newCenter, RIGHT);

		if((tempCenter.x - newCenter.x) > (viewPrintSize - footPrintSize) / 2 ) {
			newCenter.x += (viewPrintSize - footPrintSize) / 2;
		}
		else {
			newCenter = tempCenter;
		}
	}

	return newCenter;
}

Point RoomCoverage::findSquarePlacement(Point startingPoint, int direction)
{
    Point newCenter = startingPoint;
    Point controlPoint = startingPoint;

	if(direction == OVER) {
        for(double i = startingPoint.y - 1; i >= startingPoint.y - viewPrintSize; i--) {
            controlPoint.y = i;
            if(scanViewPrintEdge(controlPoint, direction)) {
                if(scanFootPrintEdge(controlPoint, direction)) {
					newCenter.y = i;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	else if(direction == UNDER) {
        for(double i = startingPoint.y + 1; i <= startingPoint.y + viewPrintSize; i++) {
            controlPoint.y = i;
            if(scanViewPrintEdge(controlPoint, direction)) {
                if(scanFootPrintEdge(controlPoint, direction)) {
					newCenter.y = i;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}
	else if(direction == RIGHT) {
        for(double i = startingPoint.x + 1; i <= startingPoint.x + viewPrintSize; i++) {
            controlPoint.x = i;
            if(scanViewPrintEdge(controlPoint, direction)) {
                if(scanFootPrintEdge(controlPoint, direction)) {
					newCenter.x = i;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}	
	else if(direction == LEFT) {
        for(double i = startingPoint.x - 1; i >= startingPoint.x - viewPrintSize; i--) {
            controlPoint.x = i;
            if(scanViewPrintEdge(controlPoint, direction)) {
                if(scanFootPrintEdge(controlPoint, direction)) {
					newCenter.x = i;
				}
				else {
					break;
				}
			}
			else {
				break;
			}
		}
	}

	return newCenter;
}

bool RoomCoverage::scanViewPrintEdge(Point scanPoint, int direction)
{
	if(direction == OVER) {
        for(int x = static_cast<int>(scanPoint.x - viewPrintSize/2); x <= static_cast<int>(scanPoint.x + viewPrintSize/2); x++) {
            if(ws->getPixel(x, static_cast<int>(scanPoint.y - viewPrintSize/2)) != WALL) {
//				std::cout << "There is no wall: " << scanPoint.y << std::endl;
				return true;
			}
		}
	}
	else if(direction == UNDER) {
        for(int x = static_cast<int>(scanPoint.x - viewPrintSize/2); x <= static_cast<int>(scanPoint.x + viewPrintSize/2); x++) {
            if(ws->getPixel(x, static_cast<int>(scanPoint.y + viewPrintSize/2)) != WALL) {
//				std::cout << "There is no wall: " << scanPoint.y << std::endl;
				return true;
			}
		}
	}
	else if(direction == RIGHT) {
        for(int y = static_cast<int>(scanPoint.y - viewPrintSize/2); y <= static_cast<int>(scanPoint.y + viewPrintSize/2); y++) {
            if(ws->getPixel(static_cast<int>(scanPoint.x + viewPrintSize/2), y) != WALL) {
				return true;
			}
		}
	}
	else if(direction == LEFT) {
        for(int y = static_cast<int>(scanPoint.y - viewPrintSize/2); y <= static_cast<int>(scanPoint.y + viewPrintSize/2); y++) {
            if(ws->getPixel(static_cast<int>(scanPoint.x - viewPrintSize/2), y) != WALL) {
				return true;
			}
		}
	}
	else {
		std::cout << "Wrong direction (scanViewPrintEdge)" << std::endl;
	}
	return false;
}
	
bool RoomCoverage::scanFootPrintEdge(Point scanPoint, int direction)
{
	if(direction == OVER) {
        for(int x = static_cast<int>(scanPoint.x - footPrintSize/2); x <= static_cast<int>(scanPoint.x + footPrintSize/2); x++) {
            if(ws->getPixel(x, static_cast<int>(scanPoint.y - footPrintSize/2)) == WALL) {
				return false;
			}
		}
	}
	else if(direction == UNDER) {
        for(int x = static_cast<int>(scanPoint.x - footPrintSize/2); x <= static_cast<int>(scanPoint.x + footPrintSize/2); x++) {
            if(ws->getPixel(x, static_cast<int>(scanPoint.y + footPrintSize/2)) == WALL) {
				return false;
			}
		}
	}
	else if(direction == RIGHT) {
        for(int y = static_cast<int>(scanPoint.y - footPrintSize/2); y <= static_cast<int>(scanPoint.y + footPrintSize/2); y++) {
            if(ws->getPixel(static_cast<int>(scanPoint.x + footPrintSize/2), y) == WALL) {
				return false;
			}
		}
	}
	else if(direction == LEFT) {
        for(int y = static_cast<int>(scanPoint.y - footPrintSize/2); y <= static_cast<int>(scanPoint.y + footPrintSize/2); y++) {
            if(ws->getPixel(static_cast<int>(scanPoint.x - footPrintSize/2), y) == WALL) {
				return false;
			}
		}
	}
	else {
		std::cout << "Wrong direction (scanFootPrintEdge)" << std::endl;
		return false;
	}
	return true;
}

void RoomCoverage::findOrthogonalNeighbors(square *homeSquare) 
{
	square *neighborSquare;

	for(int direction = OVER; direction <= LEFT; direction++) {
		double closestDistance = viewPrintSize + 1;
		neighborSquare = nullptr;

		for(auto squaresInTheRoomIt = squaresInTheRoom.begin(); squaresInTheRoomIt != squaresInTheRoom.end(); squaresInTheRoomIt++) {
			if(direction == OVER) {
				if((*squaresInTheRoomIt)->center.x == homeSquare->center.x) {
				//is located on same line
					if(	(*squaresInTheRoomIt)->center.y <= homeSquare->center.y && (*squaresInTheRoomIt)->center.y >= homeSquare->center.y - viewPrintSize && (*squaresInTheRoomIt)->center.y != homeSquare->center.y) {
					//is in neighbor range
						if(closestDistance > homeSquare->center.y - (*squaresInTheRoomIt)->center.y) {
						//is the closest neighbor yet.
							neighborSquare = *squaresInTheRoomIt;
							closestDistance = homeSquare->center.y - (*squaresInTheRoomIt)->center.y;
						}
					}
				}
			}
			else if(direction == UNDER) {
				if((*squaresInTheRoomIt)->center.x == homeSquare->center.x) {
					if(	(*squaresInTheRoomIt)->center.y >= homeSquare->center.y && (*squaresInTheRoomIt)->center.y <= homeSquare->center.y + viewPrintSize && (*squaresInTheRoomIt)->center.y != homeSquare->center.y) {
						if(closestDistance > (*squaresInTheRoomIt)->center.y - homeSquare->center.y) {
							neighborSquare = *squaresInTheRoomIt;
							closestDistance = (*squaresInTheRoomIt)->center.y - homeSquare->center.y;
						}
					}
				}
			}
			else if(direction == RIGHT) {
				if((*squaresInTheRoomIt)->center.y == homeSquare->center.y) {
					if(	(*squaresInTheRoomIt)->center.x >= homeSquare->center.x && (*squaresInTheRoomIt)->center.x <= homeSquare->center.x + viewPrintSize && (*squaresInTheRoomIt)->center.x != homeSquare->center.x) {
						if(closestDistance > (*squaresInTheRoomIt)->center.x - homeSquare->center.x) {
							neighborSquare = *squaresInTheRoomIt;
							closestDistance = (*squaresInTheRoomIt)->center.x - homeSquare->center.x;
						}
					}
				}
			}			
			else if(direction == LEFT) {
				if((*squaresInTheRoomIt)->center.y == homeSquare->center.y) {
					if(	(*squaresInTheRoomIt)->center.x <= homeSquare->center.x && (*squaresInTheRoomIt)->center.x >= homeSquare->center.x - viewPrintSize && (*squaresInTheRoomIt)->center.x != homeSquare->center.x) {
						if(closestDistance > homeSquare->center.x - (*squaresInTheRoomIt)->center.x) {
							neighborSquare = *squaresInTheRoomIt;
							closestDistance = homeSquare->center.x - (*squaresInTheRoomIt)->center.x;
						}
					}
				}
			}
		}

	//make neighbor connection
		if(neighborSquare != nullptr) {
			if(direction == OVER) {
				if(checkForClearPath(homeSquare->center, neighborSquare->center, OVER)) {
					if(homeSquare->adjacentSquares[OVER] != nullptr) {
						neighborSquare->adjacentSquares[OVER] = homeSquare->adjacentSquares[OVER];
						neighborSquare->adjacentSquares[OVER]->adjacentSquares[UNDER] = neighborSquare;
					}
					homeSquare->adjacentSquares[OVER] = neighborSquare;
					neighborSquare->adjacentSquares[UNDER] = homeSquare;
				}
			}
			else if(direction == UNDER) {
				if(checkForClearPath(homeSquare->center, neighborSquare->center, UNDER)) {
					if(homeSquare->adjacentSquares[UNDER] != nullptr) {
						neighborSquare->adjacentSquares[UNDER] = homeSquare->adjacentSquares[UNDER];
						neighborSquare->adjacentSquares[UNDER]->adjacentSquares[OVER] = neighborSquare;
					}
					homeSquare->adjacentSquares[UNDER] = neighborSquare;
					neighborSquare->adjacentSquares[OVER] = homeSquare;
				}
			}
			else if(direction == RIGHT) {
				if(checkForClearPath(homeSquare->center, neighborSquare->center, RIGHT)) {
					if(homeSquare->adjacentSquares[RIGHT] != nullptr) {
						neighborSquare->adjacentSquares[RIGHT] = homeSquare->adjacentSquares[RIGHT];
						neighborSquare->adjacentSquares[RIGHT]->adjacentSquares[LEFT] = neighborSquare;
					}
					homeSquare->adjacentSquares[RIGHT] = neighborSquare;
					neighborSquare->adjacentSquares[LEFT] = homeSquare;
				}
			}
			else if(direction == LEFT) {
				if(checkForClearPath(homeSquare->center, neighborSquare->center, LEFT)) {
					if(homeSquare->adjacentSquares[LEFT] != nullptr) {
						neighborSquare->adjacentSquares[LEFT] = homeSquare->adjacentSquares[LEFT];
						neighborSquare->adjacentSquares[LEFT]->adjacentSquares[RIGHT] = neighborSquare;
					}
					homeSquare->adjacentSquares[LEFT] = neighborSquare;
					neighborSquare->adjacentSquares[RIGHT] = homeSquare;
				}
			}	
		}
	}
//	std::cout << "Yes I updated all orthogonal junctions without crashing!" << std::endl;
}

void RoomCoverage::findDiagonalNeighbors(square *homeSquare)
{
	if(homeSquare->goodSquare) {
		for(int direction = OVER; direction <= LEFT; direction++) {
			if(homeSquare->adjacentSquares[direction] != nullptr) {
				if(homeSquare->adjacentSquares[direction]->goodSquare == true) {
					if(direction == OVER) {
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT]->goodSquare)
								homeSquare->adjacentSquares[OVERRIGHT] = homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT];
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT]->goodSquare)
								homeSquare->adjacentSquares[OVERLEFT] = homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT];
					}
					else if(direction == RIGHT) {
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[UNDER] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[UNDER]->goodSquare)
								homeSquare->adjacentSquares[UNDERRIGHT] = homeSquare->adjacentSquares[direction]->adjacentSquares[UNDER];
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[OVER] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[OVER]->goodSquare)
								homeSquare->adjacentSquares[OVERRIGHT] = homeSquare->adjacentSquares[direction]->adjacentSquares[OVER];
					}
					else if(direction == UNDER) {
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT]->goodSquare)
								homeSquare->adjacentSquares[UNDERLEFT] = homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT];
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT]->goodSquare)
								homeSquare->adjacentSquares[UNDERRIGHT] = homeSquare->adjacentSquares[direction]->adjacentSquares[RIGHT];
					}
					else if(direction == LEFT) {
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[OVER] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[OVER]->goodSquare)
								homeSquare->adjacentSquares[OVERLEFT] = homeSquare->adjacentSquares[direction]->adjacentSquares[OVER];
						if(homeSquare->adjacentSquares[direction]->adjacentSquares[UNDER] != nullptr)
							if(homeSquare->adjacentSquares[direction]->adjacentSquares[UNDER]->goodSquare)
								homeSquare->adjacentSquares[UNDERLEFT] = homeSquare->adjacentSquares[direction]->adjacentSquares[LEFT];
					}
				}
			}
		}
	}
}


void RoomCoverage::drawViewPrintEdges()
{
	square *drawSquare;
	
	std::queue<square*> squaresToBeOutlined;
	squaresToBeOutlined.push(currentSquare);

	while(!squaresToBeOutlined.empty()) {
		drawSquare = squaresToBeOutlined.front();
		squaresToBeOutlined.pop();

		for(int nextDirection = OVER; nextDirection <= LEFT; nextDirection++) {
			bool drewPixel = false;
			if(drawSquare->adjacentSquares[nextDirection] != nullptr) {
				//draw vertical lines
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - viewPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + viewPrintSize/2)); x += static_cast<int>(viewPrintSize)) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - viewPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + viewPrintSize/2)); y++) {
                        if(ws->getPixel(x,y) != VIEWPRINTEDGE) {
                            ws->setPixel(x,y,VIEWPRINTEDGE);
							drewPixel = true;
						}
					}
				}
				//draw horizontal lines
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - viewPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + viewPrintSize/2)); x++) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - viewPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + viewPrintSize/2)); y += static_cast<int>(viewPrintSize)) {
                        if(ws->getPixel(x,y) != VIEWPRINTEDGE) {
                            ws->setPixel(x,y,VIEWPRINTEDGE);
							drewPixel = true;
						}
					}
				}

				if(drewPixel) {
					squaresToBeOutlined.push(drawSquare->adjacentSquares[nextDirection]);
				}
			}
		}
	}
}

void RoomCoverage::drawFootPrintEdges()
{
	square *drawSquare;
	
	std::queue<square*> squaresToBeOutlined;
	std::list<square*> squaresAlreadyChecked;
	squaresToBeOutlined.push(currentSquare);
	squaresAlreadyChecked.push_back(currentSquare);

	while(!squaresToBeOutlined.empty()) {
		drawSquare = squaresToBeOutlined.front();
		squaresToBeOutlined.pop();

		for(int nextDirection = OVER; nextDirection <= LEFT; nextDirection++) {
			if(drawSquare->adjacentSquares[nextDirection] != nullptr) {
				//draw vertical lines
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - footPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + footPrintSize/2)); x += static_cast<int>(footPrintSize)) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - footPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + footPrintSize/2)); y++) {
                        if(ws->getPixel(x,y) != FOOTPRINTEDGE) {
                            ws->setPixel(x,y,FOOTPRINTEDGE);
						}
					}
				}
				//draw horizontal lines
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - footPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + footPrintSize/2)); x++) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - footPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + footPrintSize/2)); y += static_cast<int>(footPrintSize)) {
                        if(ws->getPixel(x,y) != FOOTPRINTEDGE) {
                            ws->setPixel(x,y,FOOTPRINTEDGE);
						}
					}
				}

				bool shouldDrawSquare = true;
				for(auto it = squaresAlreadyChecked.begin(); it != squaresAlreadyChecked.end(); it++) {
					if(*it == drawSquare->adjacentSquares[nextDirection]) {
						shouldDrawSquare = false;
					}
				}
				if(shouldDrawSquare) {
					squaresToBeOutlined.push(drawSquare->adjacentSquares[nextDirection]);
					squaresAlreadyChecked.push_back(drawSquare->adjacentSquares[nextDirection]);
				}
			}
		}
	}
}

void RoomCoverage::drawViewPrintFill()
{
	square *drawSquare;
	
	std::queue<square*> squaresToBeOutlined;
	std::list<square*> squaresAlreadyChecked;
	squaresToBeOutlined.push(currentSquare);
	squaresAlreadyChecked.push_back(currentSquare);

	while(!squaresToBeOutlined.empty()) {
		drawSquare = squaresToBeOutlined.front();
		squaresToBeOutlined.pop();
	
		for(int nextDirection = OVER; nextDirection <= LEFT; nextDirection++) {
			if(drawSquare->adjacentSquares[nextDirection] != nullptr) {
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - viewPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + viewPrintSize/2)); x++) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - viewPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + viewPrintSize/2)); y++) {
                        if(ws->getPixel(x,y) != WALL) {
                            ws->setPixel(x,y,VIEWPRINTFILL);
						}
					}
				}
				bool shouldDrawSquare = true;
				for(auto it = squaresAlreadyChecked.begin(); it != squaresAlreadyChecked.end(); it++) {
					if(*it == drawSquare->adjacentSquares[nextDirection]) {
						shouldDrawSquare = false;
					}
				}
				if(shouldDrawSquare) {
					squaresToBeOutlined.push(drawSquare->adjacentSquares[nextDirection]);
					squaresAlreadyChecked.push_back(drawSquare->adjacentSquares[nextDirection]);
				}
			}
		}
	}
}

void RoomCoverage::drawFootPrintFill()
{
	square *drawSquare;
	
	std::queue<square*> squaresToBeOutlined;
	std::list<square*> squaresAlreadyChecked;
	squaresToBeOutlined.push(currentSquare);
	squaresAlreadyChecked.push_back(currentSquare);

	while(!squaresToBeOutlined.empty()) {
		drawSquare = squaresToBeOutlined.front();
		squaresToBeOutlined.pop();
	
		for(int nextDirection = OVER; nextDirection <= LEFT; nextDirection++) {
			if(drawSquare->adjacentSquares[nextDirection] != nullptr) {
				for(int x = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x - footPrintSize/2)); x <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.x + footPrintSize/2)); x++) {
					for(int y = static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y - footPrintSize/2)); y <= static_cast<int>((drawSquare->adjacentSquares[nextDirection]->center.y + footPrintSize/2)); y++) {
                        if(ws->getPixel(x,y) != WALL) {
                            ws->setPixel(x,y,FOOTPRINTFILL);
						}
					}
				}
				bool shouldDrawSquare = true;
				for(auto it = squaresAlreadyChecked.begin(); it != squaresAlreadyChecked.end(); it++) {
					if(*it == drawSquare->adjacentSquares[nextDirection]) {
						shouldDrawSquare = false;
					}
				}
				if(shouldDrawSquare) {
					squaresToBeOutlined.push(drawSquare->adjacentSquares[nextDirection]);
					squaresAlreadyChecked.push_back(drawSquare->adjacentSquares[nextDirection]);
				}
			}
		}
	}
}

void RoomCoverage::drawSquareConnections()
{
	square *drawSquare;
    Point drawPoint;

	std::queue<square*> squaresToBeOutlined;
	std::list<square*> squaresAlreadyChecked;
	squaresToBeOutlined.push(currentSquare);
	squaresAlreadyChecked.push_back(currentSquare);

	double changeX, changeY;

	while(!squaresToBeOutlined.empty()) {
		drawSquare = squaresToBeOutlined.front();
		squaresToBeOutlined.pop();

		for(int nextDirection = OVER; nextDirection <= OVERLEFT; nextDirection++) {
			if(drawSquare->adjacentSquares[nextDirection] != nullptr) {
				if(drawSquare->center.x - drawSquare->adjacentSquares[nextDirection]->center.x == 0) {
					changeX = 0;
				}
				else if(drawSquare->adjacentSquares[nextDirection]->center.x - drawSquare->center.x <= 0) {
					changeX = -1;
				}
				else {
					changeX = 1;
				}
							
				if(drawSquare->center.y - drawSquare->adjacentSquares[nextDirection]->center.y == 0) {
					changeY = 0;
				}
				else if(drawSquare->adjacentSquares[nextDirection]->center.y - drawSquare->center.y <= 0) {
					changeY = -1;
				}
				else {
					changeY = 1;
				}
				
                drawPoint = drawSquare->center;
				
				do {
                    ws->setPixel(static_cast<int>(drawPoint.x), static_cast<int>(drawPoint.y), SQUARECONNECT);
                    drawPoint.x += changeX;
                    drawPoint.y += changeY;
                } while((drawPoint.x != drawSquare->adjacentSquares[nextDirection]->center.x) || (drawPoint.y != drawSquare->adjacentSquares[nextDirection]->center.y));

				bool shouldDrawSquare = true;
				for(auto it = squaresAlreadyChecked.begin(); it != squaresAlreadyChecked.end(); it++) {
					if(*it == drawSquare->adjacentSquares[nextDirection]) {
						shouldDrawSquare = false;
					}
				}
				if(shouldDrawSquare) {
					squaresToBeOutlined.push(drawSquare->adjacentSquares[nextDirection]);
					squaresAlreadyChecked.push_back(drawSquare->adjacentSquares[nextDirection]);
				}
			}
		}
	}
}

int RoomCoverage::getSquareWeight(square* targetSquare)
{
	int weight = 0;
	for(int direction = OVER; direction <= OVERLEFT; direction++) {
		if(targetSquare->adjacentSquares[direction] != nullptr) {
			if(targetSquare->adjacentSquares[direction]->cleared == false) {
				weight++;
			}
		}
	}

	return weight;
}

int RoomCoverage::getClosestSquareDirection(square* targetSquare)
{
	double distance = viewPrintSize;
	int closestSquareDirection = NODIRECTION;

	for(int direction = OVER; direction <= LEFT; direction++) {
		if(targetSquare->adjacentSquares[direction] != nullptr) {
			if(direction == OVER) {
				if(distance > targetSquare->center.y - targetSquare->adjacentSquares[direction]->center.y) {
					closestSquareDirection = OVER;
					distance = targetSquare->center.y - targetSquare->adjacentSquares[direction]->center.y;
				}
			}
			if(direction == UNDER) {
				if(distance > targetSquare->adjacentSquares[direction]->center.y - targetSquare->center.y) {
					closestSquareDirection = UNDER;
					distance = targetSquare->adjacentSquares[direction]->center.y - targetSquare->center.y;
				}
			}
			if(direction == LEFT) {
				if(distance > targetSquare->center.x - targetSquare->adjacentSquares[direction]->center.x) {
					closestSquareDirection = LEFT;
					distance = targetSquare->center.x - targetSquare->adjacentSquares[direction]->center.x;
				}
			}
			if(direction == RIGHT) {
				if(distance > targetSquare->adjacentSquares[direction]->center.x - targetSquare->center.x) {
					closestSquareDirection = RIGHT;
					distance = targetSquare->adjacentSquares[direction]->center.x - targetSquare->center.x;
				}
			}
		}
	}

	return closestSquareDirection;
}


Point RoomCoverage::pathCoverRoom(Point entry, Point exit)
{
	covering = true;
	currentSquare = findNearestSquare(entry);
	exitSquare = nullptr;
    exitPoint = exit;
    entryPoint = entry;

	if(currentSquare == nullptr) {
		return exit;
	}
	else {
		currentSquare->cleared = true;
		return currentSquare->center;
	}
}

Point RoomCoverage::pathGoToDoor(Point entry, Point exit)
{
	covering = false;
	currentSquare = findNearestSquare(entry);
	exitSquare = findNearestSquare(exit);
    exitPoint = exit;
    entryPoint = entry;

	if(currentSquare == nullptr) {
		return exit;
	}
	else {
		currentSquare->cleared = true;
		return currentSquare->center;
	}
}

void RoomCoverage::findOptimalRoute()
{

}

RoomCoverage::square* RoomCoverage::findNearestSquare(Point currentLocation)
{
	square* nearestSquare;
	double bestDistance = 1000000;
//	std::cout << "Best distance: " << bestDistance << std::endl;

	for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
		if((*IT)->cleared == false) {
			//pythagoras
				double deltaX = (*IT)->center.x - currentLocation.x;
				double deltaY = (*IT)->center.y - currentLocation.y;
				double testDistance = sqrt( (deltaX*deltaX) + (deltaY*deltaY) );

			if( testDistance < bestDistance) {
				nearestSquare = *IT;
				bestDistance = testDistance;
//				std::cout << "Best distance: " << bestDistance << std::endl;
			}
		}
	}

//	std::cout << "Nearest square: (" << nearestSquare->center.x << ", " << nearestSquare->center.y << ")" << std::endl;
	return nearestSquare;
}

RoomCoverage::square* RoomCoverage::findNextSquare()
{
	square* nextSquare = nullptr;
	int directionVector;
	int direction = OVER;

	if(currentSquare != nullptr) {
		do {
			if(currentSquare->adjacentSquares[direction] != nullptr && currentSquare->adjacentSquares[direction]->cleared == false) {
				if(nextSquare != nullptr) {
					if(getSquareWeight(currentSquare->adjacentSquares[direction]) < getSquareWeight(nextSquare)) {
						//std::cout << "square weight (" << currentSquare->adjacentSquares[direction]->center.x << ", " << currentSquare->adjacentSquares[direction]->center.y << ") : ";
						//std::cout << getSquareWeight(currentSquare->adjacentSquares[direction]) << std::endl;
						nextSquare = currentSquare->adjacentSquares[direction];
						directionVector = direction;
					}
					else if(getSquareWeight(currentSquare->adjacentSquares[direction]) == getSquareWeight(nextSquare)) {
						if(directionVectorWeight[directionVector] > directionVectorWeight[direction]) {
							nextSquare = currentSquare->adjacentSquares[direction];
							directionVector = direction;
						}
					}
				}
				else {
					nextSquare = currentSquare->adjacentSquares[direction];
					directionVector = direction;
				}
			}
				direction++;
		} while(direction <= LEFT);
		
		if(covering == true) {
			if(nextSquare != nullptr) {
//				std::cout << "I am here 1" << std::endl;
//				std::cout << "square weight (" << nextSquare->center.x << ", " << nextSquare->center.y << ") : ";
//				std::cout << getSquareWeight(nextSquare) << std::endl;
				if(getSquareWeight(nextSquare) != 1) {
//					std::cout << "I am here 2" << std::endl;
					int closestSquareDirection = getClosestSquareDirection(currentSquare);
					if(closestSquareDirection != NODIRECTION && currentSquare->adjacentSquares[closestSquareDirection]->cleared == false) {
//						std::cout << "Shortest path found: (" << currentSquare->center.x << ", " << currentSquare->center.y << ")" << std::endl;
						nextSquare = currentSquare->adjacentSquares[closestSquareDirection];
					}
				}
			}
		}
	}

	return nextSquare;
}

Point RoomCoverage::getNextLocation()
{
    Point nextLocation;
	square* nextSquare;

	if(currentSquare == exitSquare && currentSquare != nullptr) {
	//found a route to the door
//		std::cout << "I found the door" << std::endl;
        nextLocation = exitPoint;
	}
	else {
	//get the next square moving towards an exit
		nextSquare = findNextSquare();
		if(nextSquare == nullptr) {
			if(currentSquare == nullptr) {
                nextSquare = findNearestSquare(entryPoint);
			}
			else {
				nextSquare = findNearestSquare(currentSquare->center);
			}
			if(nextSquare == nullptr) {
                nextLocation = exitPoint;
			}
		}
		currentSquare = nextSquare;
		if(nextSquare == nullptr) {
            nextLocation = exitPoint;
		}
		else {
			nextLocation = nextSquare->center;
		}
	}

	if(currentSquare != nullptr) {
		currentSquare->cleared = true;
	}

	return nextLocation;
}

bool RoomCoverage::roomCleared()
{
	for(auto IT = squaresInTheRoom.begin(); IT != squaresInTheRoom.end(); IT++) {
		if((*IT)->cleared == false) {
			return false;
		}
	}

	return true;
}

void RoomCoverage::cleanWorkspace() 
{
    for(int x = 0; x < ws->getWidth(); x++) {
        for(int y = 0; y < ws->getHeight(); y++) {
            if(ws->getPixel(x, y) == ROBOTPATH) {
                ws->setPixel(x, y, CLEAR);
			}
		}
	}
}


void RoomCoverage::pathTest(Point nextLocation)
{
//	std::cout << "PathTest" << std::endl;
//	std::cout << "currentSquare: (" << currentSquare->center.x << ", " << currentSquare->center.y << ")" << std::endl;
//	std::cout << "nextLocation: ("  << nextLocation.x << ", " << nextLocation.y << ")" << std::endl;

	drawPath(currentSquare->center, nextLocation);
}

void RoomCoverage::drawPath(Point startingPoint, Point endPoint)
{
	double changeX, changeY;
    Point drawPoint = startingPoint;

    if(endPoint.x - startingPoint.x == 0) {
		changeX = 0;
	}
    else if(endPoint.x - startingPoint.x <= 0) {
		changeX = -1;
	}
	else {
		changeX = 1;
	}
				
    if(endPoint.y - startingPoint.y == 0) {
		changeY = 0;
	}
    else if(endPoint.y - startingPoint.y <= 0) {
		changeY = -1;
	}
	else {
		changeY = 1;
	}
	
	do {
		distanceTravelled++;
        ws->setPixel(static_cast<int>(drawPoint.x), static_cast<int>(drawPoint.y), ROBOTPATH);
        if(drawPoint.x != endPoint.x) {
            drawPoint.x += changeX;
		}
        if(drawPoint.y != endPoint.y) {
            drawPoint.y += changeY;
		}
    } while((drawPoint.x != endPoint.x) || (drawPoint.y != endPoint.y));
}


int RoomCoverage::calculateSquareLength(int radius)
{
	//Here we are rounding down..
	return static_cast<int>((2 * (radius / ( sqrt(2) ) ) ));
	//return radius;
}


RoomCoverage::square::square(Point center)
{
	this->center = center;
	goodSquare = true;
	cleared = false;
//	std::cout << "I made a new square: (" << center.x << ", " << center.y << ")" << std::endl;

	for(int i = OVER; i <= OVERLEFT; i++) {
		adjacentSquares[i] = nullptr;
	}
}

bool RoomCoverage::square::operator==(const square& compare)
{
	if(this->center.x == compare.center.x && this->center.y == compare.center.y)
		return true;
	else
		return false;
}
