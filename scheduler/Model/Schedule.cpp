/*
 * Schedule.cpp
 *
 *  Created on: 10 mai 2016
 *      Author: dell
 */

#include <stdlib.h>
#include <time.h>
#include <iterator>

#include "config.h"
#include "Schedule.h"
#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/julian_day.h"

Schedule::Schedule() {
	// TODO Auto-generated constructor stub

}

Schedule::Schedule(vector<Observation> observations, int observation_length, int teles_length, double ln, double lat) {

	this->observations_length = observation_length;
	this->observations = observations;
	this->teles_length = teles_length;
	this->observer.lng = ln;
	this->observer.lat = lat;

	const int width = observation_length;
	const int height = N_TELESCOPE;

	this->teles_alloc_matrix = new int*[width];
	for(int i = 0; i < teles_length; i++){

		teles_alloc_matrix[i] = new int[height];
	}
}

Schedule::~Schedule() {
	// TODO Auto-generated destructor stub
}


const vector<Observation>& Schedule::getObservations() const {
	return observations;
}

void Schedule::setObservations(const vector<Observation>& observations) {
	this->observations = observations;
}


double Schedule::getTotalDuration() const {
	return total_duration;
}

void Schedule::setTotalDuration(double totalDuration) {
	total_duration = totalDuration;
}

const time_interval& Schedule::getNightHorizon() const {
	return night_horizon;
}

void Schedule::setNightHorizon(const time_interval& nightHorizon) {
	night_horizon = nightHorizon;
}

int Schedule::getObservationsLength() const {
	return observations_length;
}

void Schedule::setObservationsLength(int observationsLength) {
	observations_length = observationsLength;
}

int** Schedule::getTelesAllocMatrix() const {
	return teles_alloc_matrix;
}

void Schedule::setTelesAllocMatrix(int** telesAllocMatrix) {
	teles_alloc_matrix = telesAllocMatrix;
}

int Schedule::getTelesLength() const {
	return teles_length;
}

void Schedule::setTelesLength(int telesLength) {
	teles_length = telesLength;
}


double Schedule::calculateTelescopeDuration(int telescope){

	double teles_duration = 0;

	for( int i = 0; i < observations_length ; i++ ){

		if( teles_alloc_matrix[i][telescope] == 1 ){

			teles_duration += observations.at(i).getDuration();
		}
	}

	return teles_duration;
}


double Schedule::calculateTotalDuration() {

	double total = calculateTelescopeDuration(0);;
	double t_duration;

	for( int i = 1; i < teles_length; i++ ){

		t_duration = calculateTelescopeDuration(i);
		if( t_duration > total ){

			total = t_duration;
		}
	}

	return total;
}

int Schedule::calculateNightHorizon(double JD, struct ln_lnlat_posn* observer){

	struct ln_rst_time * solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;

	int result = ln_get_solar_rst(JD, observer, solar_rst);

	if (result == SUCCESS) {
		if( JD > solar_rst->rise ){

			struct ln_rst_time * next_solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;
			ln_get_solar_rst(JD+1, observer, next_solar_rst);

			night_horizon.start = solar_rst->set;
			night_horizon.end = next_solar_rst->rise;
		}
		else{

			night_horizon.start = JD;
			night_horizon.end = solar_rst->rise;
		}

		return SUCCESS;
	}
	else return result;
}

int Schedule::targetGenerator(Target * target){

	bool stop = false;
	int circump;

	//Target generation
	do {
		double dec = 0.0;
		double ra = 0.0;

		double ra_range = 360.0 - 0.0;
		double dec_range = 90.0 - (-90.0);

		double div = RAND_MAX / dec_range;
		dec = -90.0 + rand() / div;

		div = RAND_MAX / ra_range;
		ra = rand() / div;

		target = new Target(ra, dec);

		circump = target->get_rise_set_transit(night_horizon.start, OBSERVATORY_HORIZON, &observer);

		if( circump != -1 ) stop = true;

	} while (!stop);

	return SUCCESS;
}


int Schedule::timeConstraintGenerator(time_interval * requested, double julian_exp){

	requested = new time_interval();

	double time_const = (double) rand() / (double) RAND_MAX;
	if( time_const <= TIME_CONST_RATIO ){

		//randomly generating start time
		double range = night_horizon.end - night_horizon.start;
		double div = RAND_MAX / range;
		requested->start = night_horizon.start + rand() / div;

		//end of observation time
		requested->end = requested->start + julian_exp; //start + exposure in JD
		if( requested->end > night_horizon.end ) return FAILURE;

		return SUCCESS;
	}

	return FAILURE;
}


int Schedule::heightConstraintGenerator(double * min_height){

	double height_const = (double) rand() / (double) RAND_MAX;
	if( height_const <= MIN_HEIGHT_RATIO ){

		double range = 90.0 - 0.0;
		double div = RAND_MAX / range;
		*min_height = rand() / div;
		return SUCCESS;
	}

	return FAILURE;
}

int Schedule::moonDistConstraintGenerator(double * min_moon_dist){

	double moon_const = (double) rand() / (double) RAND_MAX;
	if( moon_const <= MIN_MOON_DIST_RATIO ){

		double range = 180.0 - MOON_DISK;
		double div = RAND_MAX / range;
		*min_moon_dist = rand() / div;
		return SUCCESS;
	}

	return FAILURE;
}

int Schedule::observationRequestGenerator(Request * request){

	int isTimeConstrainted = 1;
	int isHeightConstrainted = 1;
	int isMoonConstrainted = 1;

	srand(time(NULL));

	//Target Generation...must be observable
	Target target;
	targetGenerator(&target);

	//randomly generating exposure time
	int exp_range = MAX_EXPOSURE - MIN_EXPOSURE;
	int exp_div = RAND_MAX / exp_range;
	int exposure = MIN_EXPOSURE + rand() / exp_div;
	ln_date t;
	t.seconds = exposure;
	double julian_exp = ln_get_julian_day(&t);

	//time constraint
	time_interval * requested = new time_interval();
	isTimeConstrainted = timeConstraintGenerator(requested, julian_exp);

	//min height constraint
	double min_height = 0;
	isHeightConstrainted = heightConstraintGenerator(&min_height);

	//min moon constraint
	double min_moon_dist = MOON_DISK;
	isMoonConstrainted = moonDistConstraintGenerator(&min_moon_dist);

	double first_end = requested->start;

	for(int i = 1; i <= request->getLength(); i++){

		Observation * obs = new Observation(request, i, target, julian_exp);
		obs->setTimeConst(isTimeConstrainted);

		obs->setHeightConst(isHeightConstrainted);
		if( !isHeightConstrainted )
			obs->setMinHeight(min_height);

		obs->setMoonConst(isMoonConstrainted);
		if( !isMoonConstrainted )
			obs->setMoonMinSeparation(min_moon_dist);

		requested->start = first_end + (i-1)*(request->getPeriod());
		requested->end = requested->start + julian_exp;
		obs->setReqTime(*requested);
		first_end = requested->end;

		request->addObservation(*obs);
	}

	return SUCCESS;
}

int Schedule::singularRequestGenerator(Request * request){

	int obs_length = 1;
	int period = 0;

	srand(time(NULL));
	double multi_obs = (double) rand() / (double) RAND_MAX;
	if( multi_obs < PERIODIC_RATIO ){

		obs_length = rand() % MAX_OBS_REQ;
		period = (rand() % MAX_PERIOD) + MIN_PERIOD;
	}

	ln_date * t = new ln_date();
	t->seconds = period;

	request->setLength(obs_length);
	request->setPeriod(ln_get_julian_day(t));

	//Generating priority
	request->setPriority((double) (rand() % MAX_PRIO) + 1.0 / 10.0);

	//generating observations
	//...
	observationRequestGenerator(request);
}

int Schedule::randomObservationListGenerator(int request_length) {

	for(int i = 1; i <= request_length; i++){

		//request and observation generation
		Request request(i);
		singularRequestGenerator(&request);

		//concatenate
		observations.insert(observations.end(),
				request.getObservations().begin(),
				request.getObservations().end());
	}
}

int Schedule::randomObservationAllocation(){

	srand(time(NULL));
	double selected;

	for(int i = 0; i < observations_length; i++){

		selected = (double) rand() / (double) RAND_MAX;
		if( selected < (observations[i].getRequest()->getPriority()) ){

			observations[i].setTaken(0);

			//allocate a random telescope
			//...


			//move on to the next observation
			//...
		}
	}
}
