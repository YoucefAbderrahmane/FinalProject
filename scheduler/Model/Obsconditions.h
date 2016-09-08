/*
 * Obsconditions.h
 *
 *  Created on: 5 juil. 2016
 *      Author: root
 */

#ifndef OBSCONDITIONS_H_
#define OBSCONDITIONS_H_

#include <libnova/ln_types.h>
#include "config.h"

//#include "../genethic_algo/aures.h"

class Obs_conditions {

	friend class Schedule;
	friend class Observation;
	friend class Request;

public:
	Obs_conditions();
	Obs_conditions(double julian_day);
	virtual ~Obs_conditions();

	void setObserverFromConfig();

	int calculateNightHorizon();
	double getJd();
	void setJd(double jd);
	time_interval getNightHorizon();
	void setNightHorizon(time_interval nightHorizon);
	ln_lnlat_posn getObserver();
	ln_lnlat_posn * getObserverPtr();
	void setObserver(ln_lnlat_posn observer);
	int getAllSet();
	void setAllSet(int allSet);
	double getHorizon();
	void setHorizon(double horizon);

	double night_duration_in_ms ();

private:
	ln_lnlat_posn observer; //position of the observer
	time_interval night_horizon; //the scheduling night horizon
	double JD;	//date of the schedule
	double horizon;
	int allSet; //equal to 0 when all conditions are set correctly
};

#endif /* OBSCONDITIONS_H_ */
