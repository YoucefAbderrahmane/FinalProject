/*
 * Schedule.cpp
 *
 *  Created on: 10 mai 2016
 *      Author: dell
 */

#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "Schedule.h"
#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/julian_day.h"

Schedule::Schedule() {
	// TODO Auto-generated constructor stub

}

Schedule::Schedule(vector<Observation> observations, int observation_length, int teles_length) {

	this->observations_length = observation_length;
	this->observations = observations;
	this->teles_length = teles_length;

	const int width = observation_length;
	const int height = teles_length;

	this->teles_alloc_matrix = new int*[width];
	for(int i = 0; i < teles_length; i++){

		teles_alloc_matrix[i] = new int[height];
	}
}

Schedule::~Schedule() {
	// TODO Auto-generated destructor stub
}

const vector<t_observations>& Schedule::getObservations() const {
	return observations;
}

void Schedule::setObservations(const vector<t_observations>& observations) {
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

	t_observations teles = observations.at(telescope);

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

int Schedule::observationRequestGenerator(Request * request){

	srand(time(NULL));

	//Target generation
	double dec = 0.0;
	double ra = 0.0;

	double ra_range = 360.0 - 0.0;
	double dec_range = 90.0 - (-90.0);

	double div = RAND_MAX / dec_range;
	dec = -90.0 + rand() / div;

	div = RAND_MAX / ra_range;
	ra = rand() / div;

	Target target(ra, dec);


	//Observation parameters
	time_interval requested = new time_interval();
	double min_height = 0;
	double min_moon_dist = MOON_DISK;

	double exposure;
	double julian_exp;

	//time constraint
	double time_const = (double) rand() / (double) RAND_MAX;
	if( time_const <= TIME_CONST_RATIO ){

		//randomly generating start time
		double range = night_horizon.end - night_horizon.start;
		div = RAND_MAX / range;
		requested.start = night_horizon.start + rand() / div;

		//randomly generating exposure time
		int exp_range = MAX_EXPOSURE - MIN_EXPOSURE;
		int exp_div = RAND_MAX / exp_range;
		int exposure = MIN_EXPOSURE + rand() / exp_div;

		ln_date t;
		t.seconds = exposure;
		julian_exp = ln_get_julian_day(&t)
		//end of observation time
		requested.end = requested.start + julian_exp; //start + exposure in JD

		if( requested.end > night_horizon.end ) return FAILURE;
	}

	//min height constraint
	double height_const = (double) rand() / (double) RAND_MAX;
	if( height_const <= MIN_HEIGHT_RATIO ){

		double range = 90.0 - 0.0;
		double div = RAND_MAX / range;
		min_height = rand() / div;
	}

	//min moon constraint
	double moon_const = (double) rand() / (double) RAND_MAX;
	if( moon_const <= MIN_MOON_DIST_RATIO ){

		double range = 180.0 - MOON_DISK;
		double div = RAND_MAX / range;
		min_moon_dist = rand() / div;
	}

	double first_end = requested.start;

	for(int i = 1; i <= request->length; i++){

		Observation obs = new Observation(request, i, target, exposure);

		obs.setMinHeight(min_height);
		obs.setMoonMinSeparation(min_moon_dist);

		requested.start = first_end + (i-1)*(request->getPeriod());
		requested.end = requested.start + julian_exp;
		obs.setReqTime(requested);
		first_end = requested.end;
	}
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

	ln_date t = new ln_date();
	t.seconds = period;

	request->setLength(obs_length);
	request->setPeriod(ln_get_julian_day(&t));

	//generating observation preperties
	//...


	observationRequestGenerator(request);

	//generating observations
	//...


}

int Schedule::singularObservationGenerator(Observation * observation){


}

int Schedule::randomObservationListGenerator(int request_length) {

	int total = 0;
	for(int i = 1; i <= request_length; i++){

		//request and observation generation
		Request request(i);
		singularRequestGenerator(&request);

		//concatenate
		observations.insert(observations.end(),
				std::make_move_iterator(request.getObservations().begin()),
				std::make_move_iterator(request.getObservations().end()));
	}
}
