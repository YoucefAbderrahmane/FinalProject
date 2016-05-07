/*
 * Observation.cpp
 *
 *  Created on: 3 mai 2016
 *      Author: dell
 */

#include "Observation.h"

#include <stdlib.h>
#include <math.h>

#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/julian_day.h"

Observation::Observation(int obs_id, Target target, int exposure_time) {

	this->obs_id = obs_id;
	this->target = target;
	this->exposure_time = exposure_time;
}

Observation::~Observation() {
	// TODO Auto-generated destructor stub
}

int Observation::getExposureTime() const {
	return exposure_time;
}

void Observation::setExposureTime(int exposureTime) {
	exposure_time = exposureTime;
}

double Observation::getMinHeight() const {
	return min_height;
}

void Observation::setMinHeight(double minHeight) {
	min_height = minHeight;
}

double Observation::getMoonMinSeparation() const {
	return moon_min_separation;
}

void Observation::setMoonMinSeparation(double moonMinSeparation) {
	moon_min_separation = moonMinSeparation;
}

int Observation::getObsId() const {
	return obs_id;
}

void Observation::setObsId(int obsId) {
	obs_id = obsId;
}

const Target& Observation::getTarget() const {
	return target;
}

void Observation::setTarget(const Target& target) {
	this->target = target;
}

const struct time_interval& Observation::getReqTime() const {
	return req_time;
}

void Observation::setReqTime(const struct time_interval& reqTime) {
	req_time = reqTime;
}

const struct time_interval& Observation::getSchedTime() const {
	return sched_time;
}

void Observation::setSchedTime(const struct time_interval& schedTime) {
	sched_time = schedTime;
}

const std::vector<struct time_interval>& Observation::getVisibility() const {
	return visibility;
}

void Observation::setVisibility(
		const std::vector<struct time_interval>& visibility) {
	this->visibility = visibility;
}

//*************


int Observation::calculateVisibility(double JD,
		struct ln_lnlat_posn* observer, std::vector<struct time_interval> * visibility) {

	struct time_interval night;
	struct time_interval vis;
	struct time_interval unvis;

	/** GET night horizon **/

	//calculate rise and set of the target
	target.get_rise_set_transit(JD, observer);
	double rise = target.getRiseSetTransit().rise;
	double set = target.getRiseSetTransit().set;

	/** END **/

	struct ln_rst_time * solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;

	ln_get_solar_rst(JD, observer, solar_rst);

	if( JD > solar_rst->rise ){

		struct ln_rst_time * next_solar_rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));;
		ln_get_solar_rst(JD+1, observer, next_solar_rst);

		night.start = solar_rst->set;
		night.end = next_solar_rst->rise;
	}
	else{

		night.start = JD;
		night.end = solar_rst->rise;
	}


	if( rise < set ){

		if( set < night.start || rise > night.end ){

			(*visibility).push_back(time_interval());
			return -1;
		}

		vis.start = std::max(rise, night.start);
		vis.end = std::min(set, night.end);

		(*visibility).push_back(vis);
		return 0;
	}
	else{

		if( set > night.end || rise < night.start ){

			vis.start = night.start;
			vis.end = night.end;
			(*visibility).push_back(vis);
			return 0;
		}

		if( set < night.start && rise > night.end ){

			(*visibility).push_back(vis);
			return -1;
		}

		unvis.start = std::max(set, night.start);
		unvis.end = std::min(rise, night.end);

		if( unvis.start > night.start ){

			vis.start = night.start;
			vis.end = unvis.start;
			(*visibility).push_back(vis);
		}

		if(unvis.end < night.end ){

			vis.start = unvis.end;
			vis.end = night.end;
			(*visibility).push_back(vis);
		}

		return 0;
	}
}
