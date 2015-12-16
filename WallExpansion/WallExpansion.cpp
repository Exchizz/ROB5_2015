void Planning::wall_expansion() {
	int expansion_factor = 4;
	std::vector<std::vector <int> > expand_map;


	for(int i = 0; i < getWidth(); i++){
		std::vector<int> col(getHeight());
		expand_map.push_back(col);
	}
	for(int x = 0; x < getWidth(); x++){
		for(int y = 0; y < getHeight(); y++){
			if(getPixel(x,y) <= 50){ // Retter senere
				for(int i = 0; i<=expansion_factor;i++){
					for(int j = 0; j<=expansion_factor;j++){
						if(y+i <= getHeight()-1 && x+j <= getWidth()-1 ){
							expand_map[x+j][y+i] = 1;
						}
						if(y+i <= getHeight()-1 && x-j >= 0){
							expand_map[x-j][y+i] = 1;
						}
						if(y-i >= 0 && x-j >= 0 ){
							expand_map[x-j][y-i] = 1;
						}
						if(y-i >= 0 && x+j <= getWidth()-1 ){
							expand_map[x+j][y-i] = 1;
						}
					}
				}
			}else if(expand_map[x][y] != 1){
					expand_map[x][y] = 0;
			}
		}
	}

	std::cout << "width: " << getWidth() << std::endl;
	for(int x = 0; x < getWidth(); ++x){
		for(int y = 0; y < getHeight(); y++){
			//setPixel(x,y,(expand_map[x][y] == 0) ? WHITE : BLACK);
			setPixel(x,y,expand_map[x][y]);
		}
	}
}
