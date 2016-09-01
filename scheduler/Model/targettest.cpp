/*
 * targettest.cpp
 *
 *  Created on: 15 août 2016
 *      Author: root
 */

#include "targettest.h"

target_test::target_test(double x, double y): x(x), y(y) {
	// TODO Auto-generated constructor stub

}

target_test::~target_test() {
	// TODO Auto-generated destructor stub
}

double target_test::getX() const {
	return x;
}

void target_test::setX(double x) {
	this->x = x;
}

double target_test::getY() const {
	return y;
}

void target_test::setY(double y) {
	this->y = y;
}

double target_test::add() {
	return x+y;
}
