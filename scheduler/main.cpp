/*
 * main.cpp
 *
 *  Created on: 1 mai 2016
 *      Author: root
 */

#include <iostream>
#include "Target.h"

using namespace std;

int main(){

	Target vega(279.2347, 38.7836);

	cout << vega.getEqRAsc() + ' ' + vega.eq_dec << endl;

	return 0;
}
