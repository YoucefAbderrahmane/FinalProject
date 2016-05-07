/*
 * Observation.h
 *
 *  Created on: 3 mai 2016
 *      Author: dell
 */

#ifndef OBSERVATION_H_
#define OBSERVATION_H_

#include <iostream>
#include <vector>
#include "Target.h"

struct time_interval{
	//in Julian Day
	double start;
	double end;
};

class Observation {
public:
	Observation();
	Observation(int obs_id, Target target, int exposure_time);
	virtual ~Observation();

	int getExposureTime() const;
	void setExposureTime(int exposureTime);
	double getMinHeight() const;
	void setMinHeight(double minHeight);
	double getMoonMinSeparation() const;
	void setMoonMinSeparation(double moonMinSeparation);
	int getObsId() const;
	void setObsId(int obsId);
	const Target& getTarget() const;
	void setTarget(const Target& target);
	const struct time_interval& getReqTime() const;
	void setReqTime(const struct time_interval& reqTime);
	const struct time_interval& getSchedTime() const;
	void setSchedTime(const struct time_interval& schedTime);
	const std::vector<struct time_interval>& getVisibility() const;
	void setVisibility(const std::vector<struct time_interval>& visibility);

	//calculate possible observation time using target and Sun rise and set
	int calculateVisibility(double JD, struct ln_lnlat_posn * observer,
			std::vector<struct time_interval> * visibility);


private:
	int obs_id;
	Target target;
	int exposure_time;
	struct time_interval req_time;
	struct time_interval sched_time;
	double min_height;
	double moon_min_separation;
	std::vector<struct time_interval> visibility;
};

#endif /* OBSERVATION_H_ */
