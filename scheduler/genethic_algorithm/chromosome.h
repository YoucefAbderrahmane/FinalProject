/*
 * chromosome.h
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#ifndef CHROMOSOME_H_

#define CHROMOSOME_H_

#include "gene.h"
#include "ga_config.h"
#include "../Model/config.h"

class Observation;
class Target;

class chromosome {

	friend class population;

public:
	chromosome();
	chromosome(std::vector<Observation> * Observations);
	virtual ~chromosome();

	void compute_constraints();
	void compute_obj_func();

	double get_obj_func(int index);
	double get_cst_val(int index);

	void checkObservations();

private:
	std::vector<Observation> * observations;

	std::vector<gene> genes;
	std::vector<int> telescopes_alloc;

	std::vector<double> f;	//objective functions vector
	std::vector<double> c; //constraints vector
	std::vector<int> overlap_index; //index of observations that overlaps with their upcoming observation

	int pareto_rank; //the Pareto non domination rank (Pareto fronts)
	double crowding_dist; //the Pareto crowding distance

	int dom_count; //the Pareto domination count for the individual
	std::vector<int> dom_list; //list of dominated individuals
};

#endif /* CHROMOSOME_H_ */