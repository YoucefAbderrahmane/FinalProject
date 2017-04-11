/*
 * simplegene.h
 *
 *  Created on: 9 févr. 2017
 *      Author: root
 */

#ifndef SIMPLEGENE_H_
#define SIMPLEGENE_H_

#include "../Model/Observation.h"

class simple_gene {
public:
	simple_gene();
	virtual ~simple_gene();

	int is_in_time(Observation observation);

double start_date; //in JD
int telescope_used; //Id of the telescope used for observing the target
double is_sched; //tells if the observation is scheduled or not
};

#endif /* SIMPLEGENE_H_ */
