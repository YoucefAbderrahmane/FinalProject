/*
 * aures.cpp
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#include "aures.h"

namespace pagmo {
namespace problem {

double addSecondsToJD(double JD, double seconds){

	//convert JD to date
	time_t * d = new time_t();
	ln_get_timet_from_julian(JD, d);

	//Add seconds to d
	struct tm d_tm = *localtime(d);
	d_tm.tm_sec += seconds;

	//normalize tm and convert it to time_t
	time_t new_d = mktime(&d_tm);

	//return the equivalent julian day
	return ln_get_julian_from_timet (&new_d);
}

aures::aures(std::vector<Observation> observations, int observation_length, int teles_length) :
		base(observations_length*teles_length), observations_length(observation_length),
		telescopes_used(), teles_length(teles_length), night_horizon(), bounds(), total_duration()
{

	night_horizon.start = observations.at(0).getConditions()->getNightHorizon().start;
	night_horizon.end = observations.at(0).getConditions()->getNightHorizon().end;

	bounds.start = addSecondsToJD(night_horizon.start, -7200.0);
	bounds.end = addSecondsToJD(night_horizon.end, 7200.0);

	set_lb(bounds.start);
	set_ub(bounds.end);
}

aures::~aures() {
	// TODO Auto-generated destructor stub
}

base_ptr aures::clone() const {

	return base_ptr(new aures(*this));
}

void aures::objfun_impl(fitness_vector& f,
		const decision_vector& x) const {

	int dim = get_dimension();

//	std::vector<int> affectations;
//	affectations.reserve(observations_length);

	f[0] = 0.0; //f[0] : number of scheduled observations (maximize this value)
	f[1] = 0.0; //f[1] : Average of altitude merits of all observed targets (maximize)
	f[2] = 0.0; //f[2] : Average of telescope movement during the whole schedule (minimize)
	f[3] = 0.0; //f[3] : number of telescopes using for scheduling (minimize)

	int i = 0;
	int j = 0;
	int ind = 0;
	int used = 0;

	while(i < teles_length){

		j = 0;
		used = 0;
		while(j < observations_length){

			ind = get_decv_index(i, j);
			if(x[ind] > night_horizon.start && x[ind] < night_horizon.end ){

				affectations[j] = 1;
				used = 1;
			}
		}

		if(used == 1) telescopes_used[i] = 1;
	}

	i = 0;
	while( i < observations_length ){

		//Calculating f[0]
		//affectation[i] is equal to either 1 (scheduled) or 0 (not scheduled)
		f[0] += affectations[i];

		//Calculating f[1]
		double count = 0.0;
		if( affectations[i] == 1 ){

			count = count + 1.0;
			f[1] += observations[i].altituteMerit();
		}
		f[1] = (double) (f[1] / count);

		i++;
	}

	struct ln_equ_posn * pos;
	double dist = 0.0;
	double sum = 0.0;
	double telescope_count = 0.0;
	while( i < teles_length ){

		if ( telescopes_used[i] == 1 ) {

			telescope_count = telescope_count + 1.0;
			while( j < observations_length - 1 ){

				if( x[get_decv_index(i, j)] == 1 ){

					//calculate distance between current target and next target
					pos = new ln_equ_posn();
					pos = observations[j+1].getTarget().getEqCord();
					dist = observations[j].getTarget().getAngularDistance(pos);
					sum += dist;
				}
				j++;
			}
		}
	i++;
	}

	f[2] = sum / teles_length;
	f[3] = telescope_count;

	//Converting maximization problems into minimization problems (default)
	f[1] = -f[1];
	f[2] = -f[2];
}

void aures::compute_constraints_impl(constraint_vector& c,
		const decision_vector& x) const {

	int ind = 0;
	int i = 0;
	int j = 0;
	int count = 0;
	c[0] = 0;
	c[1] = 0;

	while(j < observations_length){

		//check if an observation is allocated only once to a telescope
		if( affectations[j] == 1 ){

			i = 0;
			count = 0;
			while(i < teles_length){

				ind = get_decv_index(i,j);
				if(x[ind] > night_horizon.start && x[ind] < night_horizon.end) count++;
				i++;
			}
			if(count > 1)
			c[0] = 1;
		}

		j++;
	}

		//check if all observations are scheduled within the night horizon c2
		//check if observation are not overlapped c3

		std::vector<double> inter = x;

		i = 0;
		for(i = 0; i < teles_length; i++){

			std::sort((i*observations_length) + inter.begin(), (i+1)*observations_length + inter.begin());
		}




		i = 0;
		c[2] = -1;
		c[3] = -1;
//		while(i < teles_length ){
//
//			if( telescopes_used[i] == 1 ){
//
//				//c2
//				if( start_time[i] < night_horizon.start || end_time[i] > night_horizon.end )
//					c[2] = 1;
//
//				//c3
//				j = 0;
//				while( j < observations_length - 1 ){
//
//					if( observations[j].calculateEndTime() > observations[j+1].getSchedTime() )
//						c[3] = 1;
//				}
//			}
//		}
}

int aures::get_decv_index(int mat_i, int mat_j) const {

	if( mat_i > 0) return mat_i*observations_length + mat_j;
	else return mat_j;
}

void aures::get_mat_index(int& mat_i, int& mat_j, int dec_ind) {

	mat_j = dec_ind % observations_length;
	mat_i = (dec_ind - mat_j) / observations_length;
}

} /* namespace  problem */
} /* namespace  pagmo */
