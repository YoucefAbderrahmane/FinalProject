/*
 * population.h
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include "chromosome.h"
#include "../Model/Schedule.h"

class population {
public:
	population(int population_size, Schedule schedule);
	population(std::vector<chromosome> individuals, int population_size);
	virtual ~population();

	void init(); //Randomly initialize the population
	void init_individual(int index);
	void init_gene(int index, int individual_index);

	void repair(chromosome * individual);
	void repair_vect_obs(std::vector<gene *> * genes);

	chromosome get_individual(int chromosome_index);
	int compare_fitness(std::vector<double> f1, std::vector<double> f2);//return true if f1 is better than f2
	void update_pareto_information(); //update the pareto info (front rank and crowding distance)
	void update_crowding_dist(); //update the crowding distance of all individuals
	std::vector< std::vector<int> > compute_pareto_fronts(); //compute the pareto fronts.
	std::vector<double> compute_ideal();	//compute the ideal objective vector
	std::vector<double> compute_nadir();	//compute the nadir objective vector
	// Update the domination list and the domination count when the individual at position n has changed
	void update_dom(int index);
	void check_init();
	void check_gene(int gene_idx, int individual_idx);

private:
	std::vector<chromosome> individuals;
	int population_size;

	std::vector<chromosome> champions;

	Schedule schedule;
};

#endif /* POPULATION_H_ */
