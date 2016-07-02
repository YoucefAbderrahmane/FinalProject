//============================================================================
// Name        : scheduler.cpp
// Author      : YoucefKhaled
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>

//#include <julian_day.h>
//#include <solar.h>
//#include <rise_set.h>
#include <libnova/libnova.h>

#include <pagmo.h>
#include <config.h>
#include <serialization.h>
#include <types.h>
#include <problem/base.h>

#include "../Model/Observation.h"
#include "../Model/Schedule.h"
#include "../Model/Request.h"

#include "../genethic_algo/SchedulingProb.h"


using namespace std;


time_interval night_horizon = time_interval();

int timeConstraintGenerator(time_interval * requested){

	requested = new time_interval();

	double time_const = (double) rand() / (double) RAND_MAX;
	if( time_const <= TIME_CONST_RATIO ){

		//randomly generating start time
		double range = night_horizon.end - night_horizon.start;
		double div = RAND_MAX / range;
		requested->start = night_horizon.start + rand() / div;

		//randomly generating start time second bound
		range = night_horizon.end - requested->start;
		div = RAND_MAX / range;
		requested->end = night_horizon.start + rand() / div;

		std::cout << "Time const " << SUCCESS << std::endl;
		return SUCCESS;
	}

	return FAILURE;
}


int heightConstraintGenerator(double * min_height){

	double height_const = (double) rand() / (double) RAND_MAX;
	if( height_const <= MIN_HEIGHT_RATIO ){

		double range = 90.0 - 0.0;
		double div = RAND_MAX / range;
		*min_height = rand() / div;
		return SUCCESS;
	}

	return FAILURE;
}

int moonDistConstraintGenerator(double * min_moon_dist){

	double moon_const = (double) rand() / (double) RAND_MAX;
	if( moon_const <= MIN_MOON_DIST_RATIO ){

		double range = 180.0 - MOON_DISK;
		double div = RAND_MAX / range;
		*min_moon_dist = rand() / div;
		return SUCCESS;
	}

	return FAILURE;
}


int main() {

	night_horizon.start = 2457568.37966;
	night_horizon.end = 2457568.67133;

	Target vega(279.2347, 38.7836);

	cout << " hi " << endl;
	Schedule * sched = new Schedule();
	sched->randomObservationListGenerator(10);

	cout << " hi again " << endl;
	cout << fixed << sched->getObservations().at(1).getExposureTime() << endl;


//	Target kochab(222.676375, 74.1555);
//
//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);
//
//	Schedule * test = new Schedule();
//
//	Request * request = new Request(1);
//
//	test->observationRequestGenerator(&request);
//
//		Schedule * sched = new Schedule();
//		sched->randomObservationAllocation();
//		cout << sched->getObservations().at(0).getReqTime().start << endl;

//		Request * request = new Request(0);
//		int obs_length = 1;
//		int period = 0;
//
//		srand(time(NULL));
//		double multi_obs = (double) rand() / (double) RAND_MAX;
//		//multi_obs = 0.1;
//		if( multi_obs < PERIODIC_RATIO ){
//
//			obs_length = rand() % MAX_OBS_REQ;
//			period = (rand() % MAX_PERIOD) + MIN_PERIOD;
//
//			request->setPeriod(period);
//		}
//
//		request->setLength(obs_length);
//
//		//Generating priority
//		request->setPriority((double) ((rand() % MAX_PRIO) + 1.0) / 10.0);
//
//		//generating observations
//		//...
//
//		int isTimeConstrainted = 1;
//			int isHeightConstrainted = 1;
//			int isMoonConstrainted = 1;
//
//			srand(time(NULL));
//
//			//Target Generation...must be observable
//
//
//			//randomly generating exposure time
//			int exp_range = MAX_EXPOSURE - MIN_EXPOSURE;
//			int exp_div = RAND_MAX / exp_range;
//			int exposure = MIN_EXPOSURE + rand() / exp_div;
//			ln_date * ti = new ln_date();
//			ti->seconds = exposure;
//			double julian_exp = ln_get_julian_day(ti);
//
//			//time constraint
//			time_interval * requested = new time_interval();
//			isTimeConstrainted = timeConstraintGenerator(requested);
//			cout << isTimeConstrainted << endl;
//
//			//min height constraint
//			double min_height = 0;
//			isHeightConstrainted = heightConstraintGenerator(&min_height);
//
//			//min moon constraint
//			double min_moon_dist = MOON_DISK;
//			isMoonConstrainted = moonDistConstraintGenerator(&min_moon_dist);
//
//			double first_end = requested->start;
//
//			cout << "length " << fixed << request->getLength() << endl;
//			cout << "period " << fixed << request->getPeriod() << endl;
//
//			for(int j = 1; j <= request->getLength(); j++){
//
//				Observation * obs = new Observation(request, j, vega, julian_exp);
//				obs->setTimeConst(isTimeConstrainted);
//
//				obs->setHeightConst(isHeightConstrainted);
//				if( !isHeightConstrainted )
//					obs->setMinHeight(min_height);
//
//				obs->setMoonConst(isMoonConstrainted);
//				if( !isMoonConstrainted )
//					obs->setMoonMinSeparation(min_moon_dist);
//
//				requested->start = first_end + (j-1)*(request->getPeriod());
//				requested->end = requested->start + julian_exp;
//				obs->setReqTime(*requested);
//				first_end = requested->end;
//
//				request->addObservation(*obs);
//			}
//
//		cout << "start " << fixed << request->getObservations().at(0).getReqTime().start << endl;

	return 0;
}
