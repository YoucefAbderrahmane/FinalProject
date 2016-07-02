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
	//Constructors and destructors
	Observation();
	Observation(Request * request, int obs_id, Target target, double exposure_time);
	virtual ~Observation();

	//Getters and Setters
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
	int getTelescope() const;
	void setTelescope(int telescope);
	int getTaken() const;
	void setTaken(int taken);

	void setTimeConst(int isConst);
	int isTimeConst();
	void setHeightConst(int isConst);
	int isHeightConst();
	void setMoonConst(int isConst);
	int isMoonConst();

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
	double getSchedTime() const;
	void setSchedTime(double schedTime);
	double getEndTime() const;
	void setEndTime(double endTime);

private:
	Request * request; //The request whome the observation belong to
	int obs_id;	//observation id from DB
	Target target;	//target of the observation
	double exposure_time;	//exposure time needed for the shot
	struct time_interval req_time;	//requested time interval for the start of the observation
	double sched_time;	//scheduled start time
	double end_time;	//scheduled end time
	double min_height;	//minimum height allowed
	double moon_min_separation;	//minimum moon distance allowed
	int telescope; //telescope used for this observation if taken = 0
	int taken; // 0 if it is scheduled, 1 else
	int constraints[3]; //0 is constraint exists, 1 if not
};

#endif /* OBSERVATION_H_ */