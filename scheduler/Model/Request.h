/*
 * Request.h
 *
 *  Created on: 30 avr. 2016
 *      Author: root
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#ifndef REQUEST_H_
#define REQUEST_H_

using namespace std;

class Request {
private:
	Target target;
	vector <int> intes;
	vector <Observation> obs;
	bool state;

public:
	Request();
	virtual ~Request();
};

#endif /* REQUEST_H_ */
