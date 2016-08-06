/*
 * Obsconditions.cpp
 *
 *  Created on: 5 juil. 2016
 *      Author: root
 */

#include <Obsconditions.h>

Obs_conditions::Obs_conditions() : observer(), night_horizon(), horizon(), allSet(1) {
	// TODO Auto-generated constructor stub

	JD = ln_get_julian_from_sys();
	setObserverFromConfig();
}

Obs_conditions::Obs_conditions(double julian_day) : observer(), night_horizon(), JD(julian_day),
		horizon(OBSERVATORY_HORIZON), allSet(0) {
	// TODO Auto-generated constructor stub

	setObserverFromConfig();
	calculateNightHorizon();
}

Obs_conditions::~Obs_conditions() {
	// TODO Auto-generated destructor stub
}

void Obs_conditions::setObserverFromConfig(){

	observer.lat = LATITUDE;
	observer.lng = LONGITUDE;
}

int Obs_conditions::calculateNightHorizon(){

	struct ln_rst_time * solar_rst = new ln_rst_time();

	int result = ln_get_solar_rst(JD, &observer, solar_rst);

	if (result == SUCCESS) {
		if( JD < solar_rst->set ){

			struct ln_rst_time * next_solar_rst = new ln_rst_time();
			ln_get_solar_rst(JD+1, &observer, next_solar_rst);

			night_horizon.start = solar_rst->set;
			night_horizon.end = next_solar_rst->rise;
		}
		else{

			night_horizon.start = JD;
			night_horizon.end = solar_rst->rise;
		}

		allSet = 0;

		return SUCCESS;
	}
	else return FAILURE;
}

double Obs_conditions::getJd() const {
	return JD;
}

void Obs_conditions::setJd(double jd) {
	JD = jd;
}

const time_interval& Obs_conditions::getNightHorizon() const {
	return night_horizon;
}

void Obs_conditions::setNightHorizon(const time_interval& nightHorizon) {
	night_horizon = nightHorizon;
}

const ln_lnlat_posn& Obs_conditions::getObserver() const {
	return observer;
}

void Obs_conditions::setObserver(const ln_lnlat_posn& observer) {
	this->observer = observer;
}

int Obs_conditions::getAllSet() const {
	return allSet;
}

void Obs_conditions::setAllSet(int allSet) {
	this->allSet = allSet;
}

double Obs_conditions::getHorizon() const {
	return horizon;
}

void Obs_conditions::setHorizon(double horizon) {
	this->horizon = horizon;
}
