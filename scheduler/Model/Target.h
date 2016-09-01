/*
 * Target.h
 *
 *  Created on: 2 mai 2016
 *      Author: dell
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <ctime>
#include <time.h>

//#include <libnova/libnova.h>
//#include "../genethic_algo/SchedulingProb.h"
//
//#include <libnova/julian_day.h>
//#include <libnova/transform.h>
#include <libnova/libnova.h>

class ScheduleProb;
class Observation;

class Target {

friend class ScheduleProb;
friend class Observation;

public:
	Target();
	Target(double, double);
	virtual ~Target();

	double getEqDec() const;
	void setEqDec(double eqDec);
	double getEqRAsc() const;
	void setEqRAsc(double eqRAsc);
	const struct ln_rst_time& getRiseSetTransit() const;
	void setRiseSetTransit(const struct ln_rst_time& riseSetTransit);
	
	struct ln_equ_posn * getEqCord() const;

	double getMoonAngDist();
	void setMoonAngDist(double moonAngDist);

	int get_rise_set_transit(double julian_day,
			double horizon, struct ln_lnlat_posn * observer);
	double getAngularDistance(struct ln_equ_posn * position) const;

	double getMoonAngularDistance(double JD);

private:
	double eq_r_asc;
	double eq_dec;
	struct ln_rst_time rise_set_transit;
	double moon_ang_dist;
};

#endif /* TARGET_H_ */
