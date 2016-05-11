//============================================================================
// Name        : scheduler.cpp
// Author      : YoucefKhaled
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>

#include "../libnova/libnova/julian_day.h"
#include "../libnova/libnova/solar.h"
#include "../libnova/libnova/rise_set.h"
#include "../Model/Observation.h"
#include "../Model/Schedule.h"


using namespace std;

int main() {

	Target vega(279.2347, 38.7836);

	Target kochab(222.676375, 74.1555);

	Target RigilKentaurus(219.8795833, -60.81952778);

	Target * Aldebaran = new Target(68.98041667, 16.50841667);

	Schedule sched();



	return 0;
}
