/*
 * config.h
 *
 *  Created on: 7 mai 2016
 *      Author: dell
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define SUCCESS 0
#define FAILURE 1

#define OBSERVATORY_HORIZON		19.4333

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


struct time_interval{
	//in Julian Day
	double start;
	double end;
};

#endif /* CONFIG_H_ */
