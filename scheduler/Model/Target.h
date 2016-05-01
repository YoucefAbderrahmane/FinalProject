/*
 * Target.h
 *
 *  Created on: 30 avr. 2016
 *      Author: root
 */

#ifndef TARGET_H_
#define TARGET_H_

#include <libnova/libnova/transform.h>

using namespace std;

class Target {
private:
	double eq_r_asc;
	double eq_dec;
	//vector of visiblity time

public:
	Target();
	Target(double, double)
	virtual ~Target();

	double getEqDec() const {
		return eq_dec;
	}

	void setEqDec(double eqDec) {
		eq_dec = eqDec;
	}

	double getEqRAsc() const {
		return eq_r_asc;
	}

	void setEqRAsc(double eqRAsc) {
		eq_r_asc = eqRAsc;
	}
};

#endif /* TARGET_H_ */
