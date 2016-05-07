/*
 * Target.h
 *
 *  Created on: 2 mai 2016
 *      Author: dell
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <ctime>
#include <time.h>

#include "../libnova/libnova/julian_day.h"
#include "../libnova/libnova/transform.h"


class Target {

public:
	Target();
	Target(double, double);
	virtual ~Target();

	double getEqDec() const;
	void setEqDec(double eqDec);
	double getEqRAsc() const;
	void setEqRAsc(double eqRAsc);

	const struct ln_rst_time& getRiseSetTransit() const;
	void setRiseSetTransit(const struct ln_rst_time& riseSetTransit);

	int get_rise_set_transit(double julian_day,
			struct ln_lnlat_posn * observer);


private:
	double eq_r_asc;
	double eq_dec;
	struct ln_rst_time rise_set_transit;
};

#endif /* TARGET_H_ */
