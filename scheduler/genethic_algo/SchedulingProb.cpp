/*
 * SchedulingProb.cpp
 *
 *  Created on: 1 juin 2016
 *      Author: root
 */

#include "SchedulingProb.h"

namespace pagmo {
namespace problem {

SchedulingProb::SchedulingProb(std::vector<Observation> observations,
		int observations_length,
		//int** teles_alloc_matrix,
		int teles_length,
		time_interval night_horizon,
		ln_lnlat_posn observer) : base(observations_length*teles_length),
				observations(observations),
				observations_length(observations_length),
				teles_length(teles_length),
				night_horizon(night_horizon),
				observer(observer){
	// TODO Auto-generated destructor stub

	set_lb(0);
	set_ub(observations_length);
}

SchedulingProb::~SchedulingProb() {
	// TODO Auto-generated destructor stub
}




base_ptr SchedulingProb::clone() const {

	return base_ptr(new SchedulingProb(*this));
}

void SchedulingProb::objfun_impl(fitness_vector& f,
		const decision_vector& x) const {

//	int observed = 0;
//	int i = 0, j = 0;
//
//	while(j < observations_length){
//
//		i++;
//		j = 0;
//		observed = 0;
//		while(observed == 0 && j < teles_length){
//
//			if(teles_alloc_matrix[i][j] == 1){
//
//				observed = 1;
//				f[0]++;
//			}
//			j++;
//		}
//	}

}


void SchedulingProb::compute_constraints_impl(constraint_vector& c,
		const decision_vector& x) const {

	decision_vector::size_type n_obs = getObservationsLength();

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

				if(x[get_decv_index(i,j)] > 0) count++;
				i++;
			}
			if(count > 1)
				c[0] = 1;
		}

		//check if observation is not allocated to a telescope when it is not chosen
		if( affectations[j] == 0){

			if(x[get_decv_index(i,j)] != 0)
				c[1] = 1;
		}

		j++;
	}

	//check if all observations are scheduled within the night horizon c2
	//check if observation are not overlapped c3
	i = 0;
	c[2] = -1;
	c[3] = -1;
	while(i < teles_length ){

		if( telescopes_used[i] == 1 ){

			//c2
			if( start_time[i] < night_horizon.start || end_time[i] > night_horizon.end )
				c[2] = 1;

			//c3
			j = 0;
			while( j < observations_length - 1 ){

				//if( observations[j].getSchedTime().end > observations[j+1].getSchedTime() )
					//c[3] = 1;
			}
		}
	}

}


const time_interval& SchedulingProb::getNightHorizon() const {
	return night_horizon;
}

void SchedulingProb::setNightHorizon(const time_interval& nightHorizon) {
	night_horizon = nightHorizon;
}

const std::vector<Observation>& SchedulingProb::getObservations() const {
	return observations;
}

void SchedulingProb::setObservations(
		const std::vector<Observation>& observations) {
	this->observations = observations;
}

int SchedulingProb::getObservationsLength() const {
	return observations_length;
}

void SchedulingProb::setObservationsLength(int observationsLength) {
	observations_length = observationsLength;
}

const ln_lnlat_posn& SchedulingProb::getObserver() const {
	return observer;
}

void SchedulingProb::setObserver(const ln_lnlat_posn& observer) {
	this->observer = observer;
}

int SchedulingProb::getTelesLength() const {
	return teles_length;
}

void SchedulingProb::setTelesLength(int telesLength) {
	teles_length = telesLength;
}

double SchedulingProb::getTotalDuration() const {
	return total_duration;
}

void SchedulingProb::setTotalDuration(double totalDuration) {
	total_duration = totalDuration;
}

int SchedulingProb::get_decv_index(int mat_i, int mat_j) const {

	if( mat_i > 0) return mat_i*observations_length + mat_j;
	else return mat_j;
}

void SchedulingProb::get_mat_index(int& mat_i, int& mat_j, int dec_ind) {

	mat_j = dec_ind % observations_length;
	mat_i = (dec_ind - mat_j) / observations_length;
}

} /* namespace problem */
} /* namespace pagmo */