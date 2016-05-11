/*
 * Target.cpp
 *
 *  Created on: 2 mai 2016
 *      Author: dell
 */

#include "Target.h"

#include <stdlib.h>

#include "../libnova/libnova/rise_set.h"
#include "../libnova/libnova/angular_separation.h"
#include "../libnova/libnova/lunar.h"


Target::Target() {
	// TODO Auto-generated constructor stub

}

Target::Target(double eq_ra, double eq_dec){

	this->eq_dec = eq_dec;
	this->eq_r_asc = eq_ra;
}

Target::~Target() {
	// TODO Auto-generated destructor stub
}

double Target::getEqDec() const{

	return eq_dec;
}

void Target::setEqDec(double eqDec){

	eq_dec = eqDec;
}

double Target::getEqRAsc() const {

	return eq_r_asc;
}

void Target::setEqRAsc(double eqRAsc){

	eq_r_asc = eqRAsc;
}


const struct ln_rst_time& Target::getRiseSetTransit() const {
	return rise_set_transit;
}

void Target::setRiseSetTransit(const struct ln_rst_time& riseSetTransit) {
	rise_set_transit = riseSetTransit;
}

double Target::getMoonAngDist() {
	return moon_ang_dist;
}

void Target::setMoonAngDist(double moonAngDist) {
	moon_ang_dist = moonAngDist;
}



int Target::get_rise_set_transit(double julian_day,
			double horizon, struct ln_lnlat_posn * observer){

	struct ln_equ_posn * object = (struct ln_equ_posn *) malloc(sizeof(struct ln_equ_posn));

	object->dec = getEqDec();
	object->ra = getEqRAsc();

	struct ln_rst_time * rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));

	int res = ln_get_object_next_rst_horizon(julian_day, observer, object, horizon, rst);

	setRiseSetTransit(*rst);

	return res;
}


double Target::getMoonAngularDistance(double JD) {

	struct ln_equ_posn position;

	struct ln_equ_posn * object = (struct ln_equ_posn *) malloc(sizeof(struct ln_equ_posn));
	object->dec = this->getEqDec();
	object->ra = this->getEqRAsc();

	ln_get_lunar_equ_coords (JD, &position);

	double dist = ln_get_angular_separation(&position, object);

	setMoonAngDist(dist);

	return getMoonAngDist();
}
