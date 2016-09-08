/*
 * Stats.h
 *
 *  Created on: 5 sept. 2016
 *      Author: root
 */

#ifndef STATS_H_
#define STATS_H_

class Stats {
public:
	Stats();
	virtual ~Stats();

	double f_scheduled_obs_ratio;
	int f_telescopes_used;
	double f_distance;
	double f_height;

	double time_management_ratio;
	double opt_height_ratio;
	double scheduled_in_time_ratio;
	double scheduled_above_min_height_ratio;
};

#endif /* STATS_H_ */
