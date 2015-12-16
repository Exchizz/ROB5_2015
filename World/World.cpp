/*
 * World.cpp
 *
 *  Created on: Dec 16, 2015
 *      Author: exchizz
 */

#include "World.h"

World::World(Image *rawImg) {
	img = new Image(rawImg);

	//img->imageData = rawImg->imageData;
	// TODO Auto-generated constructor stub
}


void World::WallExpansion(){
		int expansion_factor = 4;
		std::vector<std::vector <int> > expand_map;


		//Might optimize later

		for(int i = 0; i < img->getWidth(); i++){
			std::vector<int> col(img->getHeight());
			expand_map.push_back(col);
		}

		for(int x = 0; x < img->getWidth(); x++){
			for(int y = 0; y < img->getHeight(); y++){
				if(img->getPixel(x,y) <= 50){ // Retter 50 senere
					for(int i = 0; i <= expansion_factor;i++){
						for(int j = 0; j <= expansion_factor;j++){
							if(y+i <= img->getHeight()-1 && x+j <= img->getWidth()-1 ){
								expand_map[x+j][y+i] = 1;
							}
							if(y+i <= img->getHeight()-1 && x-j >= 0){
								expand_map[x-j][y+i] = 1;
							}
							if(y-i >= 0 && x-j >= 0 ){
								expand_map[x-j][y-i] = 1;
							}
							if(y-i >= 0 && x+j <= img->getWidth()-1 ){
								expand_map[x+j][y-i] = 1;
							}
						}
					}
				}else if(expand_map[x][y] != 1){
						expand_map[x][y] = img->maxValue;
				}
			}
		}

		for(int x = 0; x < img->getWidth(); ++x){
			for(int y = 0; y < img->getHeight(); y++){
				//img->setPixel(x,y,(expand_map[x][y] == 0) ? WHITE : BLACK);
				img->setPixel(x,y,expand_map[x][y]);
			}
		}
}
