/*
 * population.h
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#ifndef POPULATION_H_
#define POPULATION_H_

#include <typeinfo>
#include <limits>
#include "chromosome.h"
#include "../Model/Schedule.h"
#include <cstdio>
#include <fstream>
#include "Telescope.h"

class population {

	friend class MyNSGA;

public:
	population(int population_size, Schedule schedule, int n);
	population(std::vector<chromosome> individuals, int population_size, int n);
	virtual ~population();

	void init(); //Randomly initialize the population
	void init_individual(int index);
	void init_gene(int index, int individual_index,int nb_max_t);

	void repair(chromosome * individual);
	void repair_vect_obs(std::vector<gene *> * genes);

	void addIndividual(chromosome ch);

	int compare_fitness(std::vector<double> f1, std::vector<double> f2);//return true if f1 is better than f2 /*ok*/
	void update_pareto_information(); //update the pareto info (front rank and crowding distance)/*ok*/
	void update_crowding_dist(std::vector<chromosome *> front); /* ok *///update the crowding distance of all individuals /*ok*/
	void compute_pareto_fronts(std::vector<int> index); //compute the pareto fronts./*ok*/
	std::vector<double> compute_ideal();	//compute the ideal objective vector
	std::vector<double> compute_nadir();	//compute the nadir objective vector
	// Update the domination list and the domination count when the individual at position n has changed
	void update_dom(int index);
	void updateViolation();
	void clearChampions();
	void check_init();
	void check_gene(int gene_idx, int individual_idx);

	int get_size();
	void setSolution();// call it after using nsga2 algorithm
	void saveInFile();

	const std::vector<std::vector<int> >& getFronts() const {
		return fronts;
	}

	void setFronts(const std::vector<std::vector<int> >& fronts) {
		this->fronts = fronts;
	}

	const std::vector<chromosome> getIndividuals() const {
		return individuals;
	}

	void setIndividuals( std::vector<chromosome> individuals) {
		this->individuals = individuals;
	}
	chromosome  get_individual(int chromosome_index);
	chromosome * getIndividualPointeur(int index);
	double getDomCountOf(int index);
	int getFrontsSize();
	void bestIndividuals(int nb_champ);
	std::vector<chromosome> * getIndividualsPointer();
	 std::vector<chromosome> getChampions()  {
		return champions;
	}

	void setChampions(const std::vector<chromosome>& champions) {
		this->champions = champions;
	}
	void change();


	void displayFronts();

	 Schedule getSchedule() {
		return schedule;
	}

	void setSchedule( Schedule schedule) {
		this->schedule = schedule;
	}

	std::vector<chromosome> * get_best_front();
	chromosome compute_ideal_ind();
	void show_stats();

	 chromosome getOrdo() {
		return ordo;
	}

	void setOrdo(const chromosome ordo) {
		this->ordo = ordo;
	}
	void RetreiveRequestTelescopes(std::vector<Telescope> * telescopes);


private:
	std::vector<chromosome> individuals;
	int population_size;
	std::vector<std::vector<int> > fronts;

	std::vector<chromosome> champions;



	Schedule schedule;
	chromosome ordo;
	int n_telescopes;
};

#endif /* POPULATION_H_ */
