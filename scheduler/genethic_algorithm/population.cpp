/*
 * population.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "population.h"

#include <stdlib.h>
#include <algorithm>
#include <time.h>
#include "../Model/config.h"
#include "../utilities/time_calculation.h"

population::population(int population_size, Schedule schedule) : individuals(), population_size(population_size),
		champions(), schedule(schedule)
{
	// TODO Auto-generated constructor stub

	this->individuals.resize(population_size);
	init();
}

population::population(std::vector<chromosome *> individuals, int population_size) : individuals(individuals),
		population_size(population_size), champions()
{
	// TODO Auto-generated constructor stub
	this->individuals = individuals;
}

population::~population() {
	// TODO Auto-generated destructor stub
}

void population::init() {

	srand(time(NULL));


	//initialize all individuals of the population
	std::vector<gene>::size_type size = individuals.size();
	std::cout<<"__________________size of individuals"<< size<<std::endl;
	std::vector<chromosome>::size_type ind = 0;
	while( ind < size){

		std::cout<<"I am individual ->"<< ind <<std::endl;

		init_individual((int)ind);
		//individuals[i]->compute_obj_func();
		ind++;
		std::cout<<"I am individual the next ->"<< ind <<std::endl;
	}

	std::cout << " it is ok pop" <<std::endl;


}

void population::init_individual(int index) {

	individuals[index] = new chromosome(schedule.getObs());

	//initialize the genes of the chromosome
	std::vector<gene>::size_type size = individuals[index]->genes.size();
	for(std::vector<gene>::size_type i = 0; i < size; i++){

		init_gene(i, index);
	}

	std::cout << " End fo the initializatoin of  "<< index <<std::endl;

	individuals[index]->compute_obj_func();

	std::cout << " in inir_individual it is ok obj fun" <<std::endl;

}

void population::init_gene(int gene_index, int individual_index) {

	double inf = 0.0;	//the lower bound for the random number
	double sup = 0.0;	//the upper bound for the random number
	double range = 0.0; //needs to be equal to: sup - inf
	double div = 0.0;	//the divisor needed to generate a random number

	//generate the probability of selecting the observation
	sup = 1.0;
	range = sup - inf;
	div = RAND_MAX / range;
	individuals[individual_index]->genes[gene_index].random_selection = inf + rand() / div;

	//generate a random start date within the night horizon
	inf = schedule.getConditions()->getNightHorizon().start;
	sup = schedule.getConditions()->getNightHorizon().end;
	range = sup - inf;
	div = RAND_MAX / range;
	individuals[individual_index]->genes[gene_index].start_date = inf + rand() / div;
	individuals[individual_index]->genes[gene_index].duration = schedule.getObs()->at(gene_index).getDuration();

	//randomly chose the allocated telescope
	int low = 1;
	int up = N_TELESCOPE;
	individuals[individual_index]->genes[gene_index].telescope_used = rand() % up + low;

}

void population::check_init() {

	//randomly picking an individual from the population
	int i = rand() % individuals.size();

	//check the first, last and a random gene
	check_gene(0, i);

	std::cout << std::endl;
	std::cout << std::endl;

	int gene = rand() % individuals[0]->genes.size();
	check_gene(gene, i);

	std::cout << std::endl;
	std::cout << std::endl;

	gene = individuals[i]->genes.size() - 1;
	check_gene(gene, i);

	std::cout << std::endl;
	std::cout << std::endl;
}

chromosome * population::get_individual(int ind_index) {

	return individuals[ind_index];
}


struct gene_less_comp{

	bool operator()(gene * g1, gene * g2){

		return (g1->get_start_time() < g2->get_start_time());
	}
};

void population::repair(chromosome * individual) {

	//Organize observation by telescope
	std::vector< std::vector<gene *> > obs_per_tel;
	obs_per_tel.resize(N_TELESCOPE);

	for(std::vector<gene>::size_type i = 0; i < individual->genes.size(); i++){

		obs_per_tel[individual->genes[i].telescope_used].push_back(&individual->genes[i]);
	}

	//Sort all generated vectors of observations
	for(std::vector< std::vector<gene> >::size_type i = 0; i < obs_per_tel.size(); i++){

		std::sort(obs_per_tel[i].begin(), obs_per_tel[i].end(), gene_less_comp());
		repair_vect_obs(&obs_per_tel[i]);
	}
}

void population::repair_vect_obs(std::vector<gene *>* genes) {

	int done = 0;
	double lower_bound = schedule.getConditions()->getNightHorizon().start;
	double upper_bound = 0.0;
	double range = 0.0;
	double div = 0.0;

	for(std::vector<gene *>::size_type i = 0; i < genes->size() - 1; i++){

		if( genes->at(i)->get_end_time() > genes->at(i+1)->get_start_time() ){
			//There is an overlapping

			//1- Try to translate the observation i to the left in the timeline
				//This is possible if:
					// lower_bound + duration(i) < end(i+1)

			if( addSecondsToJD(lower_bound, genes->at(i)->duration) < genes->at(i+1)->get_start_time() ){

				//
				upper_bound = addSecondsToJD(genes->at(i+1)->get_start_time(), -genes->at(i)->duration);

				//Randomly generate the new start time between lower_bound and upper_bound
				range = upper_bound - lower_bound;
				div = RAND_MAX / range;
				genes->at(i)->start_date = lower_bound + rand() / div;

				done = 1;
			}

			//2- If 1 is not possible, try to translate observation i+1 to the right

			else if(done == 0){

				if( addSecondsToJD(genes->at(i)->get_end_time(), genes->at(i+1)->duration) < upper_bound ){

					if(i <= genes->size()-3) upper_bound = genes->at(i+2)->get_start_time();
					else upper_bound = schedule.getConditions()->getNightHorizon().end;

					lower_bound = genes->at(i)->get_end_time();
					upper_bound = addSecondsToJD(upper_bound, -genes->at(i+1)->duration);

					//Randomly generate the new start time between lower_bound and upper_bound
					range = upper_bound - lower_bound;
					div = RAND_MAX / range;
					genes->at(i+1)->start_date = lower_bound + rand() / div;

					done = 1;
				}
			}

			//3- Unschedule the observation with the lowest priority
			if(done == 0){
				if(schedule.getObservations().at(genes->at(i)->index).getRequest()->getPriority()
						> schedule.getObservations().at(genes->at(i+1)->index).getRequest()->getPriority()){

					genes->at(i+1)->random_selection = 1.0;
					done = 1;
				}
				else if(schedule.getObservations().at(genes->at(i)->index).getRequest()->getPriority()
						< schedule.getObservations().at(genes->at(i+1)->index).getRequest()->getPriority()) {

					genes->at(i)->random_selection = 1.0;
					done = 1;
				}
			}


			//4- Unschedule the observation which is farther from its requested time (or the one that doesn't have one...)
			if(done == 0){

				double dist_i = ((schedule.getObservations().at(genes->at(i)->index).getReqTime().end
						+ schedule.getObservations().at(genes->at(i)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;

				double dist_next = ((schedule.getObservations().at(genes->at(i+1)->index).getReqTime().end
						+ schedule.getObservations().at(genes->at(i+1)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;

				if(dist_i > dist_next){

					genes->at(i)->random_selection = 1.0;
				}
				else{

					genes->at(i+1)->random_selection = 1.0;
				}
			}

			done = 0;
		}
	}
}
//checks if indivual1 dominates individual2 which objective functions are f1 and f2 respectively
int population::compare_fitness(std::vector<double> f1,
		std::vector<double> f2) {
	int dom =1;
	for(int i=0; i < (int) f1.size(); i++)
	{
		if(f1[i] < f2 [i]) return 0;
	}
	return dom;
}

void population::update_crowding_dist() {

}

void population::check_gene(int gene_idx, int individual_idx) {

	std::cout << "Gene: " << gene_idx << std::endl;
	std::cout << "Gene's selection probability: " << individuals[individual_idx]->genes[gene_idx].random_selection << std::endl;
	std::cout << "Gene's start time: " << fixed << individuals[individual_idx]->genes[gene_idx].start_date << std::endl;
	std::cout << "Gene's allocated telescope: " << individuals[individual_idx]->genes[gene_idx].telescope_used << std::endl;
	std::cout << std::endl;
}

void population::update_dom(int index) {
	int i;
	chromosome* cr = this->individuals[index];

	for(i =0; i < cr->dom_count; i++)// decrease the number of domination count of every dominated individual by index
	{
		this->individuals[cr->dom_list[i]]->dom_count --;
	}
	cr->dom_list.clear();
	cr->dom_count = 0;

	for(i=0;i < this->population_size;i++)
	{
		if(i!=index)
		{
			//check if i dominates index
			if(compare_fitness(this->individuals[i]->f,cr->f))
			{
				//update the domination count of index
				cr->dom_count++;
				//check if index exists in the list of dominated individual of i and add it if not
				if(std::find(this->individuals[i]->dom_list.begin(),this->individuals[i]->dom_list.end(),index)== this->individuals[i]->dom_list.end())
				{
					this->individuals[i]->dom_list.push_back(index);
				}
			}
			else {
				//remove index from the list of dominated individuals of i
				std::vector<int>::iterator x = std::find(this->individuals[i]->dom_list.begin(),this->individuals[i]->dom_list.end(),index);
				if(x !=this->individuals[i]->dom_list.end())
				{
					this->individuals[i]->dom_list.erase(x);
				}
			}
			//check if index dominates i
			if(compare_fitness(cr->f,this->individuals[i]->f))
			{
				cr->dom_list.push_back(i);
				this->individuals[i]->dom_count++;
			}
		}
	}
}
