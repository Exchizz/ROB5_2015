/*
 * DOTgraph.cpp
 *
 *  Created on: Dec 17, 2015
 *      Author: exchizz
 */

#include "DOTgraph.h"

/*
 * USAGE:
 * dot -Tps testdot.dot -o graph1.ps
 * open using:
 * gimp graph1.ps
 */

DOTgraph::DOTgraph() {

}

DOTgraph::~DOTgraph() {
	// TODO Auto-generated destructor stub
}

void DOTgraph::SaveGraph(std::string filename){
	ofstream myfile;
	myfile.open (filename.c_str());
	myfile << "digraph G {" << std::endl;
	for(unsigned int i = 0; i < edges.size(); ++i){
		myfile << edges[i].first << " -> " <<  edges[i].second << std::endl;
	}
	myfile << "}" << std::endl;
	myfile.close();
}
void DOTgraph::addEdge(std::string string1, std::string string2){
	auto elm = std::make_pair(string1, string2);
	edges.push_back(elm);

}



std::string name(Door door){
	std::stringstream ss;
	ss  << door.start.x_pos << "." << door.start.y_pos;
	std::string str = ss.str();
	//std::cout << "out: " << str << std::endl;
	return str;
}


std::string DOTgraph::visualize(Door door){
	for(auto vectors : door.adjacent){
		auto textParent = name(door);
		auto text = visualize(vectors);
		addEdge(textParent,text);
	}

	return name(door);
}
