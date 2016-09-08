/*
 * gene.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "gene.h"
#include "../utilities/time_calculation.h"

gene::gene() : index(), random_selection(), start_date(), is_sched(1), violated_const(), duration(), end_date(), telescope_used() {
	// TODO Auto-generated constructor stub

}


gene::gene(int index) : index(index), random_selection(), start_date(), is_sched(1), violated_const(), duration(), end_date(), telescope_used() {
	// TODO Auto-generated constructor stub

}

gene::~gene() {
	// TODO Auto-generated destructor stub
}

bool gene::is_scheduled(Observation observation) {

	//return ( random_selection < observation.getRequest()->getPriority());
	return getIsSched();
}

//Observation* gene::getObs() {
//
//	return observation;
//}

double gene::get_end_time() {

	return end_date = addSecondsToJD(start_date, duration);
}

double gene::get_start_time() {
	return start_date;
}

int gene::is_in_time(Observation observation) {

	if( start_date >= observation.getReqTime().start && start_date <= observation.getReqTime().end )
		return 1;
	else return 0;
}

int gene::getIsSched() const {
	return is_sched;
}

void gene::setIsSched(int isSched) {
	is_sched = isSched;
}
