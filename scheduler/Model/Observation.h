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
#include "Request.h"
#include "config.h"

class Request;
class Target;


class Observation {
public:
	Observation();
	Observation(Request * request, int obs_id, Target target, int exposure_time);
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
	int getTelescope() const;
	void setTelescope(int telescope);
	int getTaken() const;
	void setTaken(int taken);

	//calculate possible observation time using target and Sun rise and set
	int calculateVisibility(double JD, struct ln_lnlat_posn * observer,
			std::vector<struct time_interval> * visibility);
	int calculateVisibilityHorizon(double JD,
			struct ln_lnlat_posn* observer, double horizon, std::vector<struct time_interval> * visibility);
	double getDuration();

	int isAboveMinHeight(double JD);
	int isAwayFromMoon(double JD);
	int isInReqTime();
	int isOptimalHeight(double JD);
	Request* getRequest() const;
	void setRequest(Request* request);

private:
	Request * request;
	int obs_id;
	Target target;
	int exposure_time;
	struct time_interval req_time;
	struct time_interval sched_time;
	double min_height;
	double moon_min_separation;
	int telescope; //telescope used for this observation if taken = 0
	int taken; // 1 if it is scheduled, 0 else
};

#endif /* OBSERVATION_H_ */
