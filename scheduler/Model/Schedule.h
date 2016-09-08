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
#include "Obsconditions.h"
#include "config.h"

class Request;
class Observation;
class Obs_conditions;

using namespace std;

typedef vector<Observation> t_observations;

class Schedule {

	friend class chromosome;

public:
	//Constructors and destructors
	Schedule();
	Schedule(vector<Observation>, int, int);
	Schedule(vector<Observation>, int, int, double, Obs_conditions *);
	virtual ~Schedule();

	//Getters and Setters
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

	static std::vector<Observation> * getObs();

	void checkObservations();

	//Different calculations needed
	double calculateTelescopeDuration(int telescope);
	double calculateTotalDuration();
	//int calculateNightHorizon(double JD, struct ln_lnlat_posn* observer);

	//Generators for testing the scheduler
	int targetGenerator(Target * target);
	int timeConstraintGenerator(time_interval * requested);
	int heightConstraintGenerator(double * height);
	int moonDistConstraintGenerator(double * min_moon_dist);

	int observationRequestGenerator(Request * request);
	int singularRequestGenerator(Request * request);
	int singularObservationGenerator(Observation * observation);
	int randomObservationListGenerator(int length);
	int randomObservationAllocation();
	static Obs_conditions* getConditions();
	void setConditions(Obs_conditions * conditions);


	static Obs_conditions * conditions;
	static vector<Observation> observations; //all observable requests for the night of the schedule

private:

	int observations_length; //number of observable requests
	int** teles_alloc_matrix; //telescope allocation matrix of the observations
	int teles_length; //number of telescopes used
	double total_duration;	//duration of the schedule

};

#endif /* SCHEDULE_H_ */
