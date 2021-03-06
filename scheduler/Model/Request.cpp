/*
 * Request.cpp
 *
 *  Created on: 10 mai 2016
 *      Author: dell
>>>>>>> 6d8b8ad7a55c616262f643554a2d8d4584365c22
 */

#include "Request.h"

Request::Request() {
	// TODO Auto-generated constructor stub

}

Request::Request(int req_id){

	this->req_id = req_id;
	this->state = 1;
}

Request::Request(int req_id, vector<Observation> observations, int length){

	this->req_id = req_id;
	this->observations = observations;
	this->length = length;
	this->state = 1;
}


int Request::getLength() const {
	return length;
}

void Request::setLength(int length) {
	this->length = length;
}

const vector<Observation>& Request::getObservations() const {
	return observations;
}

void Request::setObservations(const vector<Observation>& observations) {
	this->observations = observations;
}

int Request::getReqId() const {
	return req_id;
}

void Request::setReqId(int reqId) {
	req_id = reqId;
}

int Request::getState() const {
	return state;
}

void Request::setState(int state) {
	this->state = state;
}

double Request::getPeriod() const {
	return period;
}

void Request::setPeriod(double period) {
	this->period = period;
}



void Request::addObservation(Observation observation){

	observations.push_back(observation);
}

double Request::getPriority() const {
	return priority;
}

void Request::setPriority(double priority) {
	this->priority = priority;
}
