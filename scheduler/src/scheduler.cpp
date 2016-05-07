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
#include "../Model/Target.h"
#include "../Model/Observation.h"


using namespace std;

int main() {

	Target vega(279.2347, 38.7836);

	Target kochab(222.676375, 74.1555);

	Target RigilKentaurus(219.8795833, -60.81952778);

	struct ln_equ_posn object;

	object.dec = vega.getEqDec();
	object.ra = vega.getEqRAsc();

	struct ln_lnlat_posn * observer = (struct ln_lnlat_posn *) malloc(sizeof(struct ln_lnlat_posn));

	observer->lat = 36.752498626709;
	observer->lng = 3.0419700145721;

	time_t now = time(0);
	struct tm tstruct;
	tstruct = *localtime(&now);

	struct ln_date current;

	ln_get_date_from_tm (&tstruct, &current);

	//current.hours = 9;

	double julian_day = ln_get_julian_day(&current);

	struct ln_rst_time * rst = (struct ln_rst_time *) malloc(sizeof(struct ln_rst_time));



	ln_get_solar_rst(julian_day, observer, rst);

		ln_get_date(rst->rise, &current);

		cout << "Sunrise :" << current.days << ":" << current.months << ":" <<
					current.years << ":" << current.hours << ":" << current.minutes
					<< ":" << current.seconds << endl;

		ln_get_date(rst->set, &current);

		cout << "Sunset :" << current.days << ":" << current.months << ":" <<
					current.years << ":" << current.hours << ":" << current.minutes
					<< ":" << current.seconds << endl;




	ln_get_object_next_rst(julian_day, observer, &object, rst);

	ln_get_date(rst->rise, &current);

	cout << "Rise :" << current.days << ":" << current.months << ":" <<
				current.years << ":" << current.hours << ":" << current.minutes
				<< ":" << current.seconds << endl;

	ln_get_date(rst->set, &current);

	cout << "Set :" << current.days << ":" << current.months << ":" <<
				current.years << ":" << current.hours << ":" << current.minutes
				<< ":" << current.seconds << endl;





	Observation obs(1, vega, 20);

	vector<struct time_interval> visibility;

	obs.calculateVisibility(julian_day, observer, &visibility);

	ln_get_date(visibility.at(0).start, &current);

	cout << "Start :" << current.days << ":" << current.months << ":" <<
			current.years << ":" << current.hours << ":" << current.minutes
			<< ":" << current.seconds << endl;

	ln_get_date(visibility.at(0).end, &current);

	cout << "End :" << current.days << ":" << current.months << ":" <<
				current.years << ":" << current.hours << ":" << current.minutes
				<< ":" << current.seconds << endl;

	return 0;
}
