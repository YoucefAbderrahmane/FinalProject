/*
 * obssimple.h
 *
 *  Created on: 6 févr. 2017
 *      Author: root
 */

#ifndef OBSSIMPLE_H_
#define OBSSIMPLE_H_

#include "../Model/Target.h"

class obs_simple {
public:
	obs_simple();
	virtual ~obs_simple();

int obs_id;
int priority;
double duration;
double lsched;
double usched;
Target target;
//double min_moon;
//double min_height;
};

#endif /* OBSSIMPLE_H_ */
