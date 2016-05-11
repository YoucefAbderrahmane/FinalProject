/*
 * Request.h
 *
 *  Created on: 10 mai 2016
 *      Author: dell
 */

#ifndef REQUEST_H_
#define REQUEST_H_

#include <vector>

using namespace std;

class Request {
public:
	Request();
	Request(int);
	Request(int, vector<Observation>, int);
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

private:
	int req_id;
	vector<Observation> observations;
	int length;
	int state;
	double period; //in julian day
};

#endif /* REQUEST_H_ */
