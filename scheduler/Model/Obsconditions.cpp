/*
 * Obsconditions.cpp
 *
 *  Created on: 5 juil. 2016
 *      Author: root
 */

#include "Obsconditions.h"
#include <iostream>
#include <cmath>

using namespace std;

Obs_conditions::Obs_conditions() : observer(), night_horizon(), horizon(), allSet(0) {
	// TODO Auto-generated constructor stub

	JD = ln_get_julian_from_sys();
	setObserverFromConfig();
	calculateNightHorizon();
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
	struct ln_rst_time * last_solar_rst = new ln_rst_time();

	double julian = std::abs(JD) - 0.5;

<<<<<<< HEAD
	//std::cout << "SOL debut " << fixed << solar_rst->set << " fin " << fixed << solar_rst->rise << std::endl;
=======
	int result = ln_get_solar_rst(julian, &observer, solar_rst);
>>>>>>> 0387e8b643872840f8158d247c8b573c76b05fbf

	//std::cout << "SOL debut " << fixed << solar_rst->set << " fin " << fixed << solar_rst->rise << std::endl;

<<<<<<< HEAD
		ln_get_solar_rst(JD + 1, &observer, last_solar_rst);
	}
	else ln_get_solar_rst(JD - 1, &observer, last_solar_rst);




	//std::cout << "NEXT SOL debut " << fixed << last_solar_rst->set << " fin " << fixed << last_solar_rst->rise << std::endl;
=======
	if( JD < solar_rst->rise ){
>>>>>>> 0387e8b643872840f8158d247c8b573c76b05fbf

		night_horizon.start = JD;
		night_horizon.end = solar_rst->rise;
	}
	else{

		ln_get_solar_rst(julian + 1, &observer, last_solar_rst);


		night_horizon.start = std::max(JD, solar_rst->set);
		night_horizon.end = last_solar_rst->rise;
	}

	//std::cout << "SOL debut " << fixed << night_horizon.start << " fin "
//					<< fixed << night_horizon.start << std::endl;

//	if( JD > solar_rst->rise && JD < solar_rst->set){
//
//		ln_get_solar_rst(JD + 1, &observer, last_solar_rst);
//	}
//	else ln_get_solar_rst(JD - 1, &observer, last_solar_rst);
//
//
//	//std::cout << "NEXT SOL debut " << fixed << last_solar_rst->set << " fin " << fixed << last_solar_rst->rise << std::endl;
//
//	if (result == SUCCESS) {
//		if( JD < last_solar_rst->set ){
//
//			night_horizon.start = last_solar_rst->set;
//			night_horizon.end = solar_rst->rise;
//		}
//		else{
//
//			night_horizon.start = JD;
//			night_horizon.end = solar_rst->rise;
//		}
//
//		night_horizon.start = solar_rst->set;
//		night_horizon.end = last_solar_rst->rise;
//
//		std::cout << "SOL debut " << fixed << night_horizon.start << " fin "
//				<< fixed << night_horizon.start << std::endl;

		allSet = 0;

		return SUCCESS;
//	}
//	else return FAILURE;
}

double Obs_conditions::getJd() {
	return JD;
}

void Obs_conditions::setJd(double jd) {
	JD = jd;
}

time_interval Obs_conditions::getNightHorizon() {
	return night_horizon;
}

void Obs_conditions::setNightHorizon(time_interval nightHorizon) {
	night_horizon = nightHorizon;
}

ln_lnlat_posn Obs_conditions::getObserver() {
	return observer;
}

ln_lnlat_posn * Obs_conditions::getObserverPtr() {
	return &observer;
}

void Obs_conditions::setObserver(ln_lnlat_posn observer) {
	this->observer = observer;
}

int Obs_conditions::getAllSet() {
	return allSet;
}

void Obs_conditions::setAllSet(int allSet) {
	this->allSet = allSet;
}

double Obs_conditions::getHorizon() {
	return horizon;
}

void Obs_conditions::setHorizon(double horizon) {
	this->horizon = horizon;
}

double Obs_conditions::night_duration_in_ms() {

	time_t * start = new time_t();
	ln_get_timet_from_julian(night_horizon.start, start);

	time_t * end = new time_t();
	ln_get_timet_from_julian(night_horizon.end, end);

	double dif = difftime(*end, *start);
	std::cout << " diff time " << fixed << dif << "night start "
			<< night_horizon.start << " night end " << night_horizon.end << std::endl;

	return difftime(*end, *start)*1000;
}
