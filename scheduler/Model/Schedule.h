/*
 * Schedule.h
 *
 *  Created on: 10 mai 2016
 *      Author: dell
 */

#ifndef SCHEDULE_H_
#define SCHEDULE_H_

#include <vector>

#include "Request.h"
#include "Observation.h"
#include "config.h"

class Request;
class Observation;

using namespace std;

typedef vector<Observation> t_observations;

class Schedule {
public:
	Schedule();
	Schedule(vector<Observation>, int, int, double, double);
	virtual ~Schedule();


	double getTotalDuration() const;
	void setTotalDuration(double totalDuration);
	const time_interval& getNightHorizon() const;
	void setNightHorizon(const time_interval& nightHorizon);
	int getObservationsLength() const;
	void setObservationsLength(int observationsLength);
	int** getTelesAllocMatrix() const;
	void setTelesAllocMatrix(int** telesAllocMatrix);
	int getTelesLength() const;
	void setTelesLength(int telesLength);
	const vector<Observation>& getObservations() const;
	void setObservations(const vector<Observation>& observations);

	double calculateTelescopeDuration(int telescope);
	double calculateTotalDuration();
	int calculateNightHorizon(double JD, struct ln_lnlat_posn* observer);

	int targetGenerator(Target * target);

	int timeConstraintGenerator(time_interval * requested, double julian_exposure);
	int heightConstraintGenerator(double * height);
	int moonDistConstraintGenerator(double * min_moon_dist);

	int observationRequestGenerator(Request * request);
	int singularRequestGenerator(Request * request);
	int singularObservationGenerator(Observation * observation);
	int randomObservationListGenerator(int length);
	int randomObservationAllocation();


private:
	vector<Observation> observations; //all observable requests for the night of the schedule
	int observations_length;
	int** teles_alloc_matrix; //telescope allocation matrix of the observations
	int teles_length; //number of telescopes used
	time_interval night_horizon;
	double total_duration;
	ln_lnlat_posn observer;
};

#endif /* SCHEDULE_H_ */
