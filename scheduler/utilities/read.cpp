/*
 * read.cpp
 *
 *  Created on: 5 févr. 2017
 *      Author: root
 */

#include "read.hpp"
#include <string>     // std::string, std::stoi
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void read_file(string file_name, vector<Observation> *requests){

	//columns
	//idscene iduser upriority quota duration jd1 jd2 ra dec

	ifstream file(file_name.c_str());
	string str;
	getline(file, str); //skip the first line, not needed

	Observation * obs;
	int i;

	while(getline(file, str)){

		obs = new Observation();

		istringstream iss(str);

		iss >> i;
		obs->setObsId(i);

//		iss >>
	}
}
