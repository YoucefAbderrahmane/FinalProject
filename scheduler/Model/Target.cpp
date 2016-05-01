/*
 * Target.cpp
 *
 *  Created on: 30 avr. 2016
 *      Author: root
 */

#include "Target.h"

Target::Target() {
	// TODO Auto-generated constructor stub

}

Target::~Target() {
	// TODO Auto-generated destructor stub
}

Target::Target(double eq_ra, double eq_dec){

	this->eq_r_asc = eq_ra;
	this->eq_dec = eq_dec;
}

