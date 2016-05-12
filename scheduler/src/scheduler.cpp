//============================================================================
// Name        : scheduler.cpp
// Author      : YoucefKhaled
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>

#include "../libnova/libnova/julian_day.h"
#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/rise_set.h"
#include "../Model/Observation.h"
#include "../Model/Schedule.h"
#include "../Model/Request.h"


using namespace std;

struct new_type{

	int bla;
	int bli;
};

int main() {

//	Target vega(279.2347, 38.7836);
//
//	Target kochab(222.676375, 74.1555);
//
//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);
//
//	Schedule * test = new Schedule();

	//Request * request = new Request(1);

	//test.observationRequestGenerator(&request);

	vector<new_type> vect1;

	vect1.push_back(new_type());
	vect1[0].bla = 0;
	vect1[0].bli = 1;
	vect1.push_back(new_type());
	vect1[1].bla = 2;
	vect1[1].bli = 3;
	vect1.push_back(new_type());
	vect1[2].bla = 4;
	vect1[2].bli = 5;

	vector<new_type>::iterator it = vect1.begin();

	cout << "vect 1"<< endl;

	while( it != vect1.end() ){

		cout << (*it).bla << endl;
		it++;
	}


	vector<new_type> vect2;

	vect1.push_back(new_type());
	vect1[3].bla = 6;
	vect1[3].bli = 7;
	vect1.push_back(new_type());
	vect1[4].bla = 8;
	vect1[4].bli = 9;

	vect1.insert(vect1.end(), vect2.begin(), vect2.end());

	it = vect1.begin();
	cout << "new vect 1" << endl;
	while( it != vect1.end() ){

		cout << (*it).bla << endl;
		it++;
	}


	return 0;
}
