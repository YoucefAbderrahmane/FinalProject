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


#include <vector>

#include "Observation.h"
#include "Target.h"
#include "config.h"

class Target;
class Observation;

using namespace std;

class Request {
public:
	Request();
	Request(int);
	Request(int req_id, vector<Observation> observations, int length);
	virtual ~Request();

	int getLength() const;
	void setLength(int length);
	const vector<Observation>& getObservations() const;
	void setObservations(const vector<Observation>& observations);
	int getReqId() const;
	void setReqId(int reqId);
	int getState() const;
	void setState(int state);
	double getPeriod() const;
	void setPeriod(double period);
	double getPriority() const;
	void setPriority(double priority);

	void addObservation(Observation observation);


private:
	int req_id;
	vector<Observation> observations;
	int length;
	int state;
	double period; //in seconds
	double priority; //defined as probability of selection in the range [0,1]
};

#endif /* REQUEST_H_ */
