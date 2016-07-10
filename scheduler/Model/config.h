/*
 * config.h
 *
 *  Created on: 7 mai 2016
 *      Author: dell
 */

#ifndef CONFIG_H_
#define CONFIG_H_


#include <libnova/ln_types.h>

#define SUCCESS 0
#define FAILURE 1

#define OBSERVATORY_HORIZON		19.4333
#define N_TELESCOPE				16
#define POINTING_TIME 2.0//seconds

#define LONGITUDE 3.0419700145721
#define LATITUDE 36.752498626709

#define MOON_DISK 30.0

#define TIME_MARGIN 0.25


#define MAX_OBS_REQ 10
#define MAX_PERIOD 7000 //seconds
#define MIN_PERIOD 300 //seconds

#define MIN_EXPOSURE 5 //seconds
#define MAX_EXPOSURE 240 //seconds = 4min

#define PERIODIC_RATIO 0.2
#define TIME_CONST_RATIO 0.2
#define MIN_HEIGHT_RATIO 0.5
#define MIN_MOON_DIST_RATIO 0.3


#define MAX_PRIO 9
#define PRIO_1_PROBA 0.9
#define PRIO_2_PROBA 0.8
#define PRIO_3_PROBA 0.7
#define PRIO_4_PROBA 0.6
#define PRIO_5_PROBA 0.5
#define PRIO_6_PROBA 0.4
#define PRIO_7_PROBA 0.3
#define PRIO_8_PROBA 0.2
#define PRIO_9_PROBA 0.1

struct time_interval{
	//in Julian Day
	double start;
	double end;
};

//ln_lnlat_posn observer{LONGITUDE, LATITUDE};

#endif /* CONFIG_H_ */
