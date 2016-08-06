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

#include "../genethic_algo/aures.h"

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
	double getJd() const;
	void setJd(double jd);
	const time_interval& getNightHorizon() const;
	void setNightHorizon(const time_interval& nightHorizon);
	const ln_lnlat_posn& getObserver() const;
	void setObserver(const ln_lnlat_posn& observer);
	int getAllSet() const;
	void setAllSet(int allSet);
	double getHorizon() const;
	void setHorizon(double horizon);

private:
	ln_lnlat_posn observer; //position of the observer
	time_interval night_horizon; //the scheduling night horizon
	double JD;	//date of the schedule
	double horizon;
	int allSet; //equal to 0 when all conditions are set correctly
};

#endif /* OBSCONDITIONS_H_ */
