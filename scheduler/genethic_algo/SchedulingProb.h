/*
 * SchedulingProb.h
 *
 *  Created on: 1 juin 2016
 *      Author: root
 */

#ifndef SCHEDULINGPROB_H_
#define SCHEDULINGPROB_H_

#include <string>

#include <config.h>
#include <serialization.h>
#include <types.h>
#include <problem/base.h>

#include "../Model/Observation.h"
#include "../Model/config.h"
//#include "schedpop.h"

namespace pagmo { namespace problem {

class __PAGMO_VISIBLE SchedulingProb : public base {

	//friend class schedpop;

public:
	SchedulingProb(std::vector<Observation> observations,
			int observations_length,
			//int** teles_alloc_matrix,
			int teles_length,
			time_interval night_horizon,
			ln_lnlat_posn observer);
	virtual ~SchedulingProb();

	base_ptr clone() const;
	void objfun_impl(fitness_vector &, const decision_vector &) const;
	void compute_constraints_impl(constraint_vector &c, const decision_vector& x) const;

	const time_interval& getNightHorizon() const;
	void setNightHorizon(const time_interval& nightHorizon);
	const std::vector<Observation>& getObservations() const;
	void setObservations(const std::vector<Observation>& observations);
	int getObservationsLength() const;
	void setObservationsLength(int observationsLength);
	const ln_lnlat_posn& getObserver() const;
	void setObserver(const ln_lnlat_posn& observer);
	int getTelesLength() const;
	void setTelesLength(int telesLength);
	double getTotalDuration() const;
	void setTotalDuration(double totalDuration);

protected:
	int get_decv_index(int mat_i, int mat_j) const;
	void get_mat_index(int& mat_i, int& mat_j, int dec_ind);

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
	    ar & boost::serialization::base_object<base>(*this);
	    ar & observations;
	    ar & observations_length;
	    //ar & teles_alloc_matrix;
	    ar & teles_length;
	    ar & end_time;
	    ar & start_time;
	    ar & night_horizon;
	    ar & observer;
	}

	mutable std::vector<Observation> observations; //Liste des requêtes observables durant la nuit
	mutable std::vector<int> affectations;
	int observations_length; //
	//mutable int** teles_alloc_matrix;
	mutable std::vector<int> telescopes_used;
	int teles_length;
	mutable std::vector<double> end_time; //end of observation schedule of each telescope
	mutable std::vector<double> start_time; //start of observation schedule of each telescope
	time_interval night_horizon;
	double total_duration;
	ln_lnlat_posn observer;
};

} /* namespace problem */
} /* namespace pagmo */

BOOST_CLASS_EXPORT_KEY(pagmo::problem::SchedulingProb)
#endif /* SCHEDULINGPROB_H_ */
