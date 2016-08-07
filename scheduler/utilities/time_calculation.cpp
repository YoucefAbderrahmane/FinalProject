/*
 * time_calculation.cpp
 *
 *  Created on: 19 juil. 2016
 *      Author: root
 */

#include "time_calculation.h"

double addSecondsToJD(double JD, double seconds){

	//convert JD to date
	time_t * d = new time_t();
	ln_get_timet_from_julian(JD, d);

	//Add seconds to d
	struct tm d_tm = *localtime(d);
	d_tm.tm_sec += seconds;

	//normalize tm and convert it to time_t
	time_t new_d = mktime(&d_tm);

	//return the equivalent julian day
	return ln_get_julian_from_timet (&new_d);
}