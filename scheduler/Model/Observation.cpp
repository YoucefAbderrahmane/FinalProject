/*
 * Observation.cpp
 *
 *  Created on: 3 mai 2016
 *      Author: YoucefKhaled
 */

#include "Observation.h"

#include <stdlib.h>
#include <math.h>

#include <libnova/solar.h>
#include <libnova/julian_day.h>
#include <libnova/transform.h>
#include "config.h"



Observation::Observation() : request(),
								obs_id(), target(), exposure_time(), moon_min_separation(), req_time(),
								sched_time(), end_time(), min_height(), telescope(), taken(), constraints(),
								conditions(){

}


Observation::Observation(Request * request, 
	int obs_id, Target target, double exposure_time, Obs_conditions * conditions) : request(request),
			obs_id(obs_id), target(target), exposure_time(exposure_time), sched_time(), min_height(),
			moon_min_separation(MOON_DISK), telescope(), taken(), end_time(), conditions(conditions){

	this->target = target;
	this->request = request;
	this->conditions = conditions;
	target.get_rise_set_transit(conditions->JD, conditions->horizon, &conditions->observer);
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
	if( moonMinSeparation > MOON_DISK ) moon_min_separation = moonMinSeparation;
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

int Observation::getTelescope() const {
	return telescope;
}

void Observation::setTelescope(int telescope) {
	this->telescope = telescope;
}

int Observation::getTaken() const {
	return taken;
}

void Observation::setTaken(int taken) {
	this->taken = taken;
}


void Observation::setTimeConst(int isConst){

	constraints[0] = isConst;
}

int Observation::isTimeConst(){

	return constraints[0];
}

void Observation::setHeightConst(int isConst){

	constraints[1] = isConst;
}

int Observation::isHeightConst(){

	return constraints[1];
}

void Observation::setMoonConst(int isConst){

	constraints[2] = isConst;
}

int Observation::isMoonConst(){

	return constraints[2];
}

//*************

int Observation::calculateVisibility(std::vector<struct time_interval> * visibility) {

	return calculateVisibilityHorizon(OBSERVATORY_HORIZON, visibility);
}

int Observation::calculateVisibilityHorizon(double horizon, std::vector<struct time_interval> * visibility) {

	struct time_interval vis;
	struct time_interval unvis;

	int circumpolar;

	/** GET night horizon **/

	//calculate rise and set of the target

	circumpolar = target.get_rise_set_transit(conditions->JD, horizon, &conditions->observer);

	double rise = target.getRiseSetTransit().rise;
	double set = target.getRiseSetTransit().set;

	/** END **/

	if( circumpolar == -1){

		return FAILURE;
	}

	//if it is always above horizon...
	if( circumpolar == 1){

		vis.start = conditions->night_horizon.start;
		vis.end = conditions->night_horizon.end;
		(*visibility).push_back(vis);
		return SUCCESS;
	}


	if( rise < set ){

		if( set < conditions->night_horizon.start || rise > conditions->night_horizon.end ){

			(*visibility).push_back(time_interval());
			return FAILURE;
		}

		vis.start = std::max(rise, conditions->night_horizon.start);
		vis.end = std::min(set, conditions->night_horizon.end);

		(*visibility).push_back(vis);
		return SUCCESS;
	}
	else{

		if( set > conditions->night_horizon.end || rise < conditions->night_horizon.start ){

			vis.start = conditions->night_horizon.start;
			vis.end = conditions->night_horizon.end;
			(*visibility).push_back(vis);
			return SUCCESS;
		}

		if( set < conditions->night_horizon.start && rise > conditions->night_horizon.end ){

			(*visibility).push_back(vis);
			return FAILURE;
		}

		unvis.start = std::max(set, conditions->night_horizon.start);
		unvis.end = std::min(rise, conditions->night_horizon.end);

		if( unvis.start > conditions->night_horizon.start ){

			vis.start = conditions->night_horizon.start;
			vis.end = unvis.start;
			(*visibility).push_back(vis);
		}

		if(unvis.end < conditions->night_horizon.end ){

			vis.start = unvis.end;
			vis.end = conditions->night_horizon.end;
			(*visibility).push_back(vis);
		}

		return SUCCESS;
	}
}

int Observation::isAboveMinHeight(double JD) {

	//get height of object

	struct ln_equ_posn eq_coord;
	struct ln_hrz_posn horiz_coord;

	eq_coord.dec = target.getEqDec();
	eq_coord.ra = target.getEqRAsc();

	ln_get_hrz_from_equ(&eq_coord, &conditions->observer, conditions->JD, &horiz_coord);

	if( horiz_coord.alt >= getMinHeight() ) return SUCCESS;
	else return FAILURE;
}

double Observation::getDuration() {


	return this->exposure_time + POINTING_TIME;
}

int Observation::isAwayFromMoon(double JD){

	double dist = target.getMoonAngularDistance(JD);

	if( dist > getMoonMinSeparation() ) return SUCCESS; //good
	else return FAILURE; //bad
}

int Observation::isInReqTime() {

	double scheduled = getSchedTime();
	time_interval requested = getReqTime();

	if( scheduled >= requested.start
			&& scheduled <= requested.end )

		return SUCCESS; //good
	else return FAILURE; //bad
}

double Observation::calculateEndTime() {

	//convert start time from JD to date
	time_t * d = new time_t();
	ln_get_timet_from_julian(this->sched_time, d);

	//Get observation duration
	double duration = getDuration(); //in seconds

	//Add duration (in seconds) to start time (or scheduled time)
	struct tm * d_tm = localtime(d);
	d_tm->tm_sec += duration;

	//normalize tm and convert it to time_t
	time_t new_d = mktime(d_tm);

	//return the equivalent julian day
	return ln_get_julian_from_timet (&new_d);
}


double Observation::altituteMerit(){

	if( isHeightConst() ){

		//get the minimum height of the target
		double H_min = min_height;

		//get the transit heinght of the target
		double transit_jd = target.getRiseSetTransit().transit;
		struct ln_hrz_posn * position = new ln_hrz_posn();
		ln_get_hrz_from_equ (target.getEqCord(),
						& conditions->observer,
						transit_jd,
						position);
		double H_t = position->alt;

		//get the actual height of the target

			//1. get the current time in julian day
		double current_jd = ln_get_julian_from_sys();

			//2. calculate the target position
		ln_get_hrz_from_equ (target.getEqCord(),
								& conditions->observer,
								current_jd,
								position);

			//3. get the target's height
		double height = position->alt;

		//calculate the altitude merit
		return (double) (height - H_min) / (H_t - H_min);
	}
	else return 1.0;
}


double Observation::altituteMerit2(){

	//minimal altitude of an object attained during 24 hours
	double Aday_min = conditions->observer.lat - 90.0 - target.getEqDec();

	//maximal altitude
	double Aday_max = 90.0 - conditions->observer.lat + target.getEqDec();

	//

	return 0.0;
}


int Observation::isOptimalHeight(double JD) {

	double transit = getTarget().getRiseSetTransit().transit;
	double scheduled = getSchedTime();

	if( transit >= scheduled - 0.4169 && transit <= scheduled + 0.4169 ) return SUCCESS;
	else return FAILURE;
	return FAILURE;
}

Request* Observation::getRequest() const {
	return request;
}

void Observation::setRequest(Request* request) {
	this->request = request;
}

double Observation::getSchedTime() const {
	return sched_time;
}

void Observation::setSchedTime(double schedTime) {
	sched_time = schedTime;
}

double Observation::getEndTime() const {
	return end_time;
}

void Observation::setEndTime(double endTime) {
	end_time = endTime;
}

Obs_conditions* Observation::getConditions() const {
	return conditions;
}

void Observation::setConditions(Obs_conditions* conditions) {
	this->conditions = conditions;
}
