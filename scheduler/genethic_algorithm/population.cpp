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
#include "Stats.h"

population::population(int population_size, Schedule schedule) : individuals(), population_size(population_size),
		champions(), schedule(schedule)
{
	// TODO Auto-generated constructor stub

	this->individuals.reserve(population_size);
	//std::cout << "********************************************" << individuals.size() << std::endl;
	for(int i = 0; i < population_size; i++){
		individuals.push_back(* new chromosome(Schedule::observations));
		//individuals.at(i).setConditions(*(this->schedule.getConditions()));
		//individuals.at(i).setConditions(*(Schedule::conditions));
	}
	//std::cout << "********************************************" << individuals.size() << std::endl;
	//std::cout<< "Population : stard init" << std::endl;
	init();
	//std::cout<< "Population : init ended" << std::endl;
}

population::population(std::vector<chromosome> individuals, int population_size) : individuals(individuals),
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
	//std::cout<<"__________________size of individuals"<< size<<std::endl;
	std::vector<chromosome>::size_type ind = 0;
	while( ind < size){

		//std::cout<<"I am individual ->"<< ind <<std::endl;
		//std::cout<< "Population inti : stard init of individ : "<< ind << std::endl;
		init_individual((int)ind);
		//individuals[i]->compute_obj_func();
		ind++;
		//std::cout<< "Population inti : next individ : "<< ind << std::endl;

		//std::cout << "index is now = " << ind << "and size is = " << size << std::endl;
	}

}

void population::init_individual(int index) {

	//individuals[index] = new chromosome(schedule.getObs());

	//std::cout << "index = " << index << std::endl;

	//initialize the genes of the chromosome
	std::vector<gene>::size_type size = individuals.at(index).genes.size();

	//std::cout << "size " << size << std::endl;
	int nb_max_t = (rand() % N_TELESCOPE) + 1;
	individuals.at(index).nb_max_t = nb_max_t;
	//std::cout<< "Population init_individual : start " << std::endl;
	for(std::vector<gene>::size_type i = 0; i < size; i++){

		init_gene(i, index, nb_max_t);
	}

	//std::cout<< "Population init_individual : before compute obj func " << std::endl;
	individuals.at(index).compute_obj_func();
	//std::cout<< "Population init_individual : before uptdate dom " << std::endl;
	//std::cout<< "Population init_individual : start update_dom"<< std::endl;
	this->update_dom(index);
	repair(&individuals.at(index));
	//std::cout<< "Population init_individual : end " << std::endl;

}

void population::init_gene(int gene_index, int individual_index, int nb_max_t) {

	double inf = 0.0;	//the lower bound for the random number
	double sup = 0.0;	//the upper bound for the random number
	double range = 0.0; //needs to be equal to: sup - inf
	double div = 0.0;	//the divisor needed to generate a random number

	//generate the probability of selecting the observation
	sup = 1.0;
	range = sup - inf;
	div = RAND_MAX / range;
	//individuals.at(individual_index).genes.at(gene_index).random_selection = inf + rand() / div;
	//std::cout<< "Population init_gene : random_selection set " << std::endl;
	//generate a random start date within the night horizon

	//individuals.at(individual_index).genes.at(gene_index).setIsSched(rand()%2);
	individuals.at(individual_index).genes.at(gene_index).setIsSched(1);
	individuals.at(individual_index).genes.at(gene_index).index = gene_index;

//	inf = schedule.getConditions()->getNightHorizon().start;
//	sup = schedule.getConditions()->getNightHorizon().end;

	//std::cout<< "Population init_gene : start date " << std::endl;

	inf = Schedule::conditions->getNightHorizon().start;
	sup = addSecondsToJD(Schedule::conditions->getNightHorizon().end, -Schedule::observations.at(gene_index).getDuration());

	range = sup - inf;
	div = RAND_MAX / range;
	individuals.at(individual_index).genes[gene_index].start_date = inf + rand() / div;
	individuals.at(individual_index).genes[gene_index].duration = schedule.getObs()->at(gene_index).getDuration();
	individuals.at(individual_index).genes[gene_index].get_end_time();

	//std::cout<< "Population init_gene : start date set " << std::endl;


	//randomly chose the allocated telescope
	//int low = 0;
	//int up = N_TELESCOPE - 1;
	//std::cout<< "nb_max " << nb_max_t << std::endl;
	individuals.at(individual_index).genes.at(gene_index).telescope_used = (rand() % nb_max_t);// up + low;
	//std::cout << "bla  " << individuals[individual_index].genes[gene_index].telescope_used << std::endl;
	//std::cout<< "Population init_gene : end " << std::endl;

}

void population::check_init() {

	//randomly picking an individual from the population
	int i = rand() % individuals.size();

	//check the first, last and a random gene
	for(int j =0; j < individuals[i].genes.size(); j++)
	{
	check_gene(j, i);
	}
	//std::cout << std::endl;
	//std::cout << std::endl;

	//int gene = rand() % individuals[0].genes.size();
	//check_gene(gene, i);

	//std::cout << std::endl;
	//std::cout << std::endl;

	//gene = individuals[i].genes.size() - 1;
	//check_gene(gene, i);

	//std::cout << std::endl;
//	std::cout << std::endl;
}

chromosome  population::get_individual(int chromosome_index) {

	return individuals[chromosome_index];
}

chromosome * population::getIndividualPointeur( int index)
{
	return &individuals[index];}

std::vector<chromosome> * population::getIndividualsPointer() {
		return &individuals;
	}


struct gene_less_comp{

	bool operator()(gene * g1, gene * g2){

		return (g1->get_start_time() < g2->get_start_time());
	}
};

void population::repair(chromosome * individual) {

	//double tstart = clock();

	//Organize observation by telescope
	int nb_max_t = individual->getNbMaxT();
//	std::cout << "NB MAAAAAAX " << nb_max_t;
	//exit(0);
	std::vector< std::vector<gene *> > obs_per_tel(nb_max_t);
	//obs_per_tel.reserve(N_TELESCOPE);

	for(std::vector<gene>::size_type i = 0; i < individual->genes.size(); i++){

		if( individual->genes[i].telescope_used >= nb_max_t) individual->genes[i].telescope_used = rand() % nb_max_t;

		int t = individual->genes[i].telescope_used;

//		std::cout << "TTTTTTTTTTTTTTT " << t << std::endl;

		obs_per_tel.at(t).push_back(&individual->genes[i]);
	}

	//std::cout << "one " << individual->genes.at(0).get_end_time() << std::endl;

	//Sort all generated vectors of observations

	//std::cout << "one " << obs_per_tel.at(0).at(0)->get_start_time() << " two " << obs_per_tel.at(0).at(0)->get_start_time() << std::endl;

	for(std::vector< std::vector<gene> >::size_type i = 0; i < nb_max_t; i++){

		//std::cout<< "vect size "<< obs_per_tel[i].size() << std::endl;
		std::sort(obs_per_tel[i].begin(), obs_per_tel[i].end(), gene_less_comp());

		//std::cout<< "Done sorting... "<< obs_per_tel.size() << std::endl;
		if( obs_per_tel[i].size() > 0) repair_vect_obs(&obs_per_tel[i]);
	}

	//std::cout << "one " << obs_per_tel.at(0).at(0)->get_end_time() << " two " << obs_per_tel.at(0).at(0)->get_end_time() << std::endl;

	//printf("REPAIR Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

}

void population::repair_vect_obs(std::vector<gene *>* genes) {

	int done = 0;
//	double lower_bound = schedule.getConditions()->getNightHorizon().start;
	double lower_bound = Schedule::conditions->getNightHorizon().start;
	double upper_bound = 0.0;
	double range = 0.0;
	double div = 0.0;
	int g = (int) genes->size();

//	std::cout<< "nombre d'itérations : "<< g<< std::endl;
	for(int i = 0; i < g - 1; i++){

		//std::cout << "reparing " << std::endl;

		if( genes->at(i)->get_end_time() > genes->at(i+1)->get_start_time() ){
			//There is an overlapping

			/*std::cout << "overlapping i+1 start " << fixed << genes->at(i+1)->get_start_time()
					<< " duration " << fixed <<  genes->at(i)->duration << " i end " << fixed <<
					genes->at(i)->get_end_time() << std::endl;*/

			//1- Try to translate the observation i to the left in the timeline
				//This is possible if:
					// lower_bound + duration(i) < end(i+1)

			int b = addSecondsToJD(lower_bound, genes->at(i)->duration) < genes->at(i+1)->get_start_time();
//			std::cout << "Condition " << b << std::endl;
			if( b ){

				//std::cout << "shifting left " << std::endl;
//				std::cout<< "cas 1 "<< i << " i+1 "<< i+1<<std::endl;

				upper_bound = addSecondsToJD(genes->at(i+1)->get_start_time(), -genes->at(i)->duration);

				//Randomly generate the new start time between lower_bound and upper_bound
				range = upper_bound - lower_bound;
				div = RAND_MAX / range;
				genes->at(i)->start_date = lower_bound + rand() / div;

//				std::cout << "overlapping settled " << fixed << genes->at(i+1)->get_start_time() << " "
//						<< genes->at(i)->get_end_time() << std::endl;

				done = 1;
			}

			//2- If 1 is not possible, try to translate observation i+1 to the right

			if (done == 0) {

				if(i <= (g-3))
					{

					upper_bound = genes->at(i+2)->get_start_time();

					}
				else upper_bound = Schedule::conditions->getNightHorizon().end;

				if( addSecondsToJD(genes->at(i)->get_end_time(), genes->at(i+1)->duration) < upper_bound ){

						//std::cout << "shifting right " << std::endl;

						//else upper_bound = schedule.getConditions()->getNightHorizon().end;
						//else upper_bound = Schedule::conditions->getNightHorizon().end;

						lower_bound = genes->at(i)->get_end_time();
						upper_bound = addSecondsToJD(upper_bound, -genes->at(i+1)->duration);

						//Randomly generate the new start time between lower_bound and upper_bound
						range = upper_bound - lower_bound;
						div = RAND_MAX / range;
						genes->at(i+1)->start_date = lower_bound + rand() / div;

						//std::cout << "overlapping settled " << fixed << genes->at(i+1)->get_start_time() << " "
//												<< genes->at(i)->get_end_time() << std::endl;

						done = 1;
				}
			}

			//3- Unschedule the observation with the lowest priority
			if(done == 0){
//				std::cout<< "cas 3 "<< std::endl;//i << " i+1 "<< i+1<<std::endl;
//				std::cout << "priority  " << std::endl;


				double p1 = schedule.getObservations().at(genes->at(i)->index).getRequest()->getPriority();
				double p2 = schedule.getObservations().at(genes->at(i+1)->index).getRequest()->getPriority();

				//std::cout << "priority " << p1 << " " << p2 << std::endl;
				if(p1 > p2){

					//genes->at(i+1)->random_selection = 1.0;

					//std::cout << "A priority " << std::endl;
					genes->at(i+1)->setIsSched(0);
					done = 1;
				}
				else if(p1<p2) {

//					genes->at(i)->random_selection = 1.0;
					//std::cout << "no priority " << std::endl;
					genes->at(i)->setIsSched(0);
					done = 1;
				}
				else{

//					std::cout<< "cas 4 "<< std::endl;//i << " i+1 "<< i+1<<std::endl;
					double zhar = rand() / RAND_MAX;
					if(zhar < 0.5) genes->at(i)->setIsSched(0);
					else genes->at(i+1)->setIsSched(0);
				}
			}


			//4- Unschedule the observation which is farther from its requested time (or the one that doesn't have one...)
//			if(done == 0){
//
//				std::cout << "random " << std::endl;
//
//
//				//std::cout << "Random " << std::endl;
//				double dist_i = ((schedule.getObservations().at(genes->at(i)->index).getReqTime().end
//						+ schedule.getObservations().at(genes->at(i)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;
//
//				double dist_next = ((schedule.getObservations().at(genes->at(i+1)->index).getReqTime().end
//						+ schedule.getObservations().at(genes->at(i+1)->index).getReqTime().start) / 2.0) - genes->at(i)->start_date;
//
//				if(dist_i > dist_next){
//
//					//std::cout << "Dist > " << std::endl;
//					genes->at(i)->random_selection = 1.0;
//				}
//				else{
//
//					//std::cout << "Dist < " << std::endl;
//					genes->at(i+1)->random_selection = 1.0;
//				}
//			}

			done = 0;
		}
	}
//	std::cout<< "repair obs : fin"<< std::endl;
}

int population::get_size() {
	return this->population_size;
}
//checks if indivual1 dominates individual2 which objective functions are f1 and f2 respectively
int population::compare_fitness(std::vector<double> f1,
		std::vector<double> f2) {
	int sup = 0;
	for(int i=0; i < (int) f1.size(); i++)
	{
		if(f1[i] > f2 [i]) return 0;
		if(f1[i] < f2[i]) sup =1;
	}
	return sup;
}


void population::check_gene(int gene_idx, int individual_idx) {

	std::cout << "Gene: " << gene_idx << std::endl;
	std::cout << "Gene's selection : " << individuals[individual_idx].genes[gene_idx].is_sched << std::endl;
	std::cout << "Gene's start time: " << fixed << individuals[individual_idx].genes[gene_idx].start_date << std::endl;
	std::cout << "Gene's allocated telescope: " << individuals[individual_idx].genes[gene_idx].telescope_used << std::endl;
	std::cout << std::endl;
}
//ok
void population::update_dom(int index) {

	//double tstart = clock();
	//std::cout<< "-------------------start update_dom, index ="<< index<< std::endl;
	//std::cout << "getting ind index " << index << std::endl;
	chromosome * cr = &(this->individuals.at(index));

	//std::cout << "got ind index " << index << std::endl;
	/*if(cr->getDomList().size() != 0){
	for(int i =0; i < (int) cr->getDomList().size(); i++)// decrease the number of domination count of every dominated individual by index
	{
		std::cout << "taille " << cr->getDomList().size() << "e nsemble de dom minus " << i << std::endl;
		this->individuals.at(cr->getDomListOf(i)).dom_count = individuals.at(cr->getDomListOf(i)).dom_count - 1;
		//std::cout<< "mise a jour	";
	}
	}*/
	cr->clearDomList();
	cr->setDomCount(0);
	int size = this->individuals.size();
	//std::cout << "update_dom pop size :"<< size<< std::endl;
	for(int i=0;i < size;i++)
	{
		//std::cout<< "I'm i = "<< i<< std::endl;
		if(index !=i)
		{
			//std::cout<< "I'm i and different from index "<< std::endl;
			//check if index is dominated by  i
			if(compare_fitness(this->individuals.at(i).getF(),cr->getF()))
			{
				//std::cout<<"result of the comparison "<< compare_fitness(this->individuals[i].getF(),cr->getF())<< std::endl;
				//update the domination count of index
				cr->incrementDomCount();


			}

			//check if index dominates i
		if(compare_fitness(cr->f,this->individuals[i].f))
			{
				cr->dom_list.push_back(i);
				//std::cout<< "I am "<< index<< " and I dominate "<< i<< std::endl;
			}
		}
	}
	cr= NULL;
	delete cr;
//	std::cout<< "-------------find de update_dom----------------"<< std::endl;

	//printf("UPDATE DOM Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

}
//ok
struct obj_fct_comp {

	obj_fct_comp(int dim):obj_fct(dim) {};
	bool operator()(chromosome * c1, chromosome * c2){

			return (c1->get_obj_func(obj_fct) < c2->get_obj_func(obj_fct));
		}
int obj_fct;
};

//updates the crowding distance of a front ok
void population::update_crowding_dist(std::vector<chromosome *> front) {
	std::vector<chromosome>::size_type taille  = front.size()-1;
	//std::cout<<"je suis taille "<< taille<< std::endl;
	obj_fct_comp f(0);
	double crow = 0.0;
	//std::cout<<"je suis PROB_DIM "<< PROB_DIM << std::endl;

	 //we loop for each objective function
	int tour =0;
	 for (int i = 0; i < PROB_DIM; ++i)
	 {
		 f.obj_fct=i;

		 std::sort(front.begin(),front.end(),f);

		 front.at(0)->setCrowdingDist(std::numeric_limits<double>::max());

		 front.at(taille)->setCrowdingDist(std::numeric_limits<double>::max());

		 //compute crowding distance
	//	 cout<< "valeur de f(" << i << ") de 0 "<< front.at(0)->get_obj_func(i)<< endl;
		// cout<< "valeur de f(" << i << ") de 0 "<< front.at(taille)->get_obj_func(i)<< endl;

		 double diff = front.at(taille)->get_obj_func(i) - front.at(0)->get_obj_func(i);
		 //cout<< diff << endl;


		 for(std::vector<chromosome>::size_type j = 1; j < taille;j++)
		 {
			 if(diff==0.0)
			 {
				 front.at(j)->setCrowdingDist(0.0);
			//	 std::cout<<"crowd a 0"<<std::endl;
			 }
			 else{
				// std::cout<< "valeur "<< (front.at(j+1)->get_obj_func(i) - front.at(j-1)->get_obj_func(i))/diff<<std::endl;
				 //std::cout<< "former crowding distance"<< front.at(j)->getCrowdingDist()<<std::endl;

				 crow = front.at(j)->getCrowdingDist() + (front.at(j+1)->get_obj_func(i) - front.at(j-1)->get_obj_func(i))/diff;

				 front.at(j)->setCrowdingDist(crow);
				 //std::cout<< "current crowdin distance " << crow <<std::endl;
			 }
		 }
		 //std::cout<<"_________________________e tour numéro"<<tour<<std::endl;
		 tour++;
	 }
	 //std::cout<<"fin"<<std::endl;

}

void population::change(){this->individuals[0].setCrowdingDist(1024);}

//have to update dom count before
void population::update_pareto_information() {

	//double tstart = clock();

	std::vector<int> list_dom_count(this->get_size(),0);
	//we need the index to update the crowding distance in the population
	std::vector<int> index;
	std::vector<chromosome *> front, S;
	for(int i =0; i < this->get_size();i++){
		this->individuals[i].setCrowdingDist(0);
		this->individuals[i].setParetoRank(0);
		list_dom_count[i] = this->individuals[i].getDomCount();

		//find first pareto front
		if(list_dom_count[i]==0) {
			front.push_back(&(this->individuals[i]));
			index.push_back(i);
		}
	}
	//to reinitalize the fronts, because changes can appear during the resolution
	this->fronts.clear();
	unsigned int rank =1;
	//we search for the other fronts

	//std::cout << "pop update pareto, front size " << front.size() << std::endl;
	while(front.size()!=0)
	{
		//update crowding distance of the current  paerto front
		population::update_crowding_dist(front);

		//std::cout << "pop update pareto, crwd done " << std::endl;
		//update crowding distance of the population
		/*for(std::vector<int>::size_type i =0; i < index.size();i++)
		{
			this->individuals[index[i]].setCrowdingDist(front[i].getCrowdingDist());
		}*/

		population::compute_pareto_fronts(index);
		index.clear();

		//std::cout << "pop update pareto, add index done " << std::endl;
		//for each individual int the front
		for(std::vector<chromosome *>::size_type i=0; i < front.size();i++)
		{
			//std::cout<< "pop pareto_info : parcours du front"<< std::endl;
			//for each individual dominated by front[i]
			for(std::vector<double>::size_type j=0; j< front[i]->getDomList().size();j++)
			{
				//std::cout<< "pop pareto_info : modification de S"<< std::endl;
				//decrease the dom_count
				list_dom_count[front[i]->getDomList()[j]]--;
				//check if there isn't an individual that dominates the individual j
				if(list_dom_count[front[i]->getDomList()[j]]==0)
				{
					S.push_back(&(this->individuals[front[i]->getDomList()[j]]));

					this->individuals[front[i]->getDomList()[j]].setParetoRank(rank);

					//save the index of the individuals to update the crowding distance in the population
					index.push_back(front[i]->getDomList()[j]);
				}
			}
		}
		//std::cout<< "test de taille de front "<< front.size()<< endl;
		front = S;
		//std::cout<< "test 2 de taille de front "<< front.size()<< endl;
		//std::cout<< "test de taille de S "<< S.size()<< endl;
		S.clear();
		//std::cout<< "test 2 de taille de S "<< S.size()<< endl;
		rank++;
	}

	//printf("PARETO Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

}

void population::compute_pareto_fronts(std::vector<int> index) {
	fronts.push_back(index);
}

std::vector<double> population::compute_ideal() {
update_pareto_information();

std::vector<double> ideal (PROB_DIM,numeric_limits<double>::max());
for(std::vector<chromosome>::size_type i = 0; i < this->individuals.size();i++ )
{
	if(this->individuals[i].getParetoRank() == 0)
	{
		for(std::vector<double>::size_type j = 0; j < ideal.size(); j++)
		{
			if(this->individuals[i].get_obj_func(j) < ideal[j])
			{
				//std::cout<< "ideal de j "<< j<< " est "<< this->individuals[i].get_obj_func(j)<< std::endl;
				ideal[j] = this->individuals[i].get_obj_func(j);
			}
		}
	}
}
return ideal;
}

std::vector<double> population::compute_nadir() {
	update_pareto_information();

	std::vector<double> nadir (PROB_DIM,- numeric_limits<double>::max());
	for(std::vector<chromosome>::size_type i = 0; i < this->individuals.size();i++ )
	{
		if(this->individuals[i].getParetoRank() == 0)
		{
			for(std::vector<double>::size_type j = 0; j < nadir.size(); j++)
			{
				if(this->individuals[i].get_obj_func(j) > nadir[j])
				{
					nadir[j] = this->individuals[i].get_obj_func(j);
				}
			}
		}
	}
	return nadir;
}

struct crowding_d_comp {
	crowding_d_comp (population *p):ps(p){};
	bool operator()(int  c1, int  c2){

			return (ps->get_individual(c1).getCrowdingDist() < ps->get_individual(c2).getCrowdingDist());
		}
	population *ps;
};
void population::bestIndividuals(int nb_champ) {

	//double tstart = clock();
	//double t2 = clock();
	//std::cout<< "bestindividuals : j'ai besoin de  "<< nb_champ<< std::endl;
	this->update_pareto_information();
	int nb_champions = 0;
	int size_front = 0;
	std::vector<int> v;
	int index;
	int size = (int) this->fronts.size();
	//std::cout<< "bestindividuals : le nombre de fronts est de "<< size<< std::endl;
	for(index = 0; index < size; index++)
	{
		//std::cout<< "bestindividuals : index est a "<< index<< std::endl;
		nb_champions = (int) this->champions.size();
		size_front = (int) this->fronts[index].size();
		//std::cout<< "bestindividuals : le nombre de champions "<< this->champions.size()<< std::endl;
		//std::cout<< "bestindividuals : le nombre d'individus dans le front "<< size_front<< std::endl;
		if( (nb_champions + size_front) <= nb_champ)
		{
			for(int i =0; i < size_front; i++)
			{
				this->champions.push_back(this->individuals[this->fronts[index][i]]);
			}
			//printf("BEST IND 1 fronts Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
		}

		else
		{
			//printf("BEST IND end of all fronts Time taken: %.5fs\n", (double)(clock() - t2)/CLOCKS_PER_SEC);
		//	tstart = clock();
		 //std::cout<< "bestindividuals : dans else "<< index<< "nombre d'individus dans le front "<<
				// this->fronts[index].size()<< std::endl;

			v = this->fronts[index];

			crowding_d_comp c(this);
			std::sort(v.begin(),v.end(),c);
			//printf("BEST IND sort Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
			//tstart = clock();
			int k =1;
			int t = (int) v.size();
			//std::cout << "bestindividuals : le nombre de champions " << this->champions.size() << std::endl;
			//std::cout << "bestindividuals : taille du front " << v.size() << std::endl;
			while(nb_champions < nb_champ)
			{
				champions.push_back(this->get_individual(v[t-k]));
				k++;
				nb_champions++;
			}
			//std::cout << "bestindividuals : le nombre de champions " << this->champions.size() << std::endl;
			//printf("BEST IND 2 Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

			break;
		}
	}
	//std::cout<< "bestindividuals : fin "<< std::endl;
	//printf("BEST IND Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
}

void population::displayFronts()
{
	for(int i = 0; i< (int) this->fronts.size(); i++)
	{
		//std::cout<<"liste des individus du front i = "<< i<< std::endl;
		int size = (int) fronts[i].size();
		for(int j = 0; j < size; j++) std::cout<< this->fronts[i][j]<< std::endl;
		std::cout<< "________________________________"<< std::endl;
	}
}

double population::getDomCountOf(int index)
{
	return  this->individuals[index].dom_count;
}

void population::addIndividual(chromosome ch) {
	this->individuals.push_back(ch);
	this->population_size ++;
}

int population::getFrontsSize() {
	return (int) this->fronts.size();
}

void population::updateViolation() {

	//double tstart = clock();

	for(int i = 0; i < population_size; i++)
	{
		//individuals.at(i).ratio_violated_const(*schedule.getConditions());
		individuals.at(i).ratio_violated_const(*Schedule::conditions);
	}

	//printf("VIOLATION Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

}

void population::clearChampions() {
	this->champions.clear();
}

std::vector<chromosome> * population::get_best_front() {

	int size = (int)fronts.at(0).size();
	std::vector<chromosome> * best_front = new std::vector<chromosome>();

	std::vector<int> first = fronts.at(0);

	for(int i; i < size; i++){

		best_front->push_back(individuals.at(first.at(i)));
	}

	return best_front;
}

Stats * get_individual_stats(chromosome c){

	Stats * s = new Stats();

	s->f_scheduled_obs_ratio = -c.get_obj_func(0);
	s->f_height = -c.get_obj_func(1);
	s->f_distance = c.get_obj_func(2);
	s->f_telescopes_used = c.get_obj_func(3);

	//std::cout << s->f_telescopes_used << std::endl;

	s->time_management_ratio = c.get_duration();
	//std::cout << "Time management" << fixed << s->time_management_ratio << std::endl;


//	std::cout << "Taux req " << s->f_scheduled_obs_ratio << std::endl;
//	std::cout << "Distance moyenne " << s->f_distance << std::endl;
//	std::cout << "Hauteur " << s->f_height << std::endl;
//	std::cout << "Télescopes " << s->f_telescopes_used << std::endl;

	return s;
}

chromosome population::compute_ideal_ind() {
update_pareto_information();

int best = 0;

std::vector<double> ideal (PROB_DIM,numeric_limits<double>::max());
for(std::vector<chromosome>::size_type i = 0; i < this->individuals.size();i++ )
{
	if(this->individuals[i].getParetoRank() == 0)
	{
			if(this->individuals[i].get_obj_func(0) < ideal[0])
			{
				//std::cout<< "ideal de j "<< j<< " est "<< this->individuals[i].get_obj_func(j)<< std::endl;
				best = i;
				ideal[0] = this->individuals[i].get_obj_func(0);
			}
	}
}
return individuals.at(best);
}


void population::show_stats() {

	std::vector<chromosome> front = *get_best_front();
	int size = front.size();

//	Stats * s;
	Stats * all = new Stats();

	chromosome best = compute_ideal_ind();

//	std::vector<double> ideals = compute_ideal();
//
//	std::cout << " ideals " << std::endl;
//	std::cout << " ideal f0 " << ideals.at(0) << std::endl;
//	std::cout << " ideal f1 " << ideals.at(1) << std::endl;
//	std::cout << " ideal f2 " << ideals.at(2) << std::endl;
//	std::cout << " ideal f3 " << ideals.at(3) << std::endl;

//	for(int i = 0; i < size; i++){
//
//		Stats * s = get_individual_stats(front.at(i));
//
//		//if( f1 > s->f_telescopes_used)
//
//		all->f_distance += s->f_distance;
//		all->f_height += s->f_height;
//		all->f_scheduled_obs_ratio += s->f_scheduled_obs_ratio;
//		all->f_telescopes_used += s->f_telescopes_used;
//		double t = s->time_management_ratio/Schedule::conditions->night_duration_in_ms();
//		//std::cout << "Time " << t << std::endl;
//		all->time_management_ratio += t;
//	}
//
//	all->f_distance /= size;
//	all->f_height /= size;
//	all->f_scheduled_obs_ratio /= size;
//	all->f_telescopes_used /= size;
//	all->time_management_ratio /= size;
//	//all->time_management_ratio /= Schedule::conditions->night_duration_in_ms();
	std::cout<< "\n les valeurs moyennes des ";
	std::cout << "Fonctions objectifs du best" << std::endl;
//	std::cout << "Taux req " << all->f_scheduled_obs_ratio << std::endl;
//	std::cout << "Distance moyenne " << all->f_distance << std::endl;
//	std::cout << "Hauteur " << all->f_height << std::endl;
//	std::cout << "Télescopes " << all->f_telescopes_used << std::endl;
//	std::cout << "Time " << all->time_management_ratio << std::endl;

		std::cout << "Taux req " << best.get_obj_func(0) << std::endl;
		std::cout << "Distance moyenne " << best.get_obj_func(2) << std::endl;
		std::cout << "Hauteur " << best.get_obj_func(1) << std::endl;
		std::cout << "Télescopes " << best.get_obj_func(3) << std::endl;
}


//Stats get_individual_stats(chromosome c){
//
//	Stats * s = new Stats();
//
//	//Observation scheduled ratio
//	s->scheduled_obs_ratio = c.get_obj_func(0);
//
//	//Telescopes used
//	s->telescopes_used = c.get_obj_func(3);
//
//	//Distance
//	s->distance = (-c.get_obj_func(2));
//
//	//Height
//	s->height = (-c.get_obj_func(1));
//
//	for(int i = 0; (int) i < c.getGenes().size(); i++){
//
//		Obs_conditions * cond = Schedule::conditions;
//
//		s->scheduled_in_time_ratio += c.is_in_req_time(i);
//		s->scheduled_above_min_height_ratio += c.is_above_min_height(*cond, i);
//		s->opt_height_ratio += c.is_close_to_meridien(i);
//		s->time_management_ratio += c.get_duration();
//	}
//
//	s->scheduled_in_time_ratio /= c.get_obj_func(0);
//	s->scheduled_above_min_height_ratio /= c.get_obj_func(0);
//	s->opt_height_ratio /= c.get_obj_func(0);
//	s->time_management_ratio /= Schedule::conditions->night_duration_in_ms();
//
//	return *s;
//}


//void population::show_stats() {
//
//	std::vector<chromosome> front = get_best_front();
//	int size = front.size();
//	//std::vector<Stats> stats(size);
//
//	Stats * the_ultimate_stats = new Stats();
//	for(int i = 0; i < size; i++){
//
//		Stats s = get_individual_stats(front.at(i));
//		the_ultimate_stats->distance += s.distance;
//		the_ultimate_stats->height += s.height;
//		the_ultimate_stats->opt_height_ratio += s.opt_height_ratio;
//		the_ultimate_stats->scheduled_above_min_height_ratio += s.scheduled_above_min_height_ratio;
//		the_ultimate_stats->scheduled_in_time_ratio += s.scheduled_in_time_ratio;
//		the_ultimate_stats->scheduled_obs_ratio += s.scheduled_obs_ratio;
//		the_ultimate_stats->telescopes_used += s.telescopes_used;
//		the_ultimate_stats->time_management_ratio += s.time_management_ratio;
//	}
//
//	the_ultimate_stats->distance /= size;
//	the_ultimate_stats->height /= size;
//	the_ultimate_stats->scheduled_obs_ratio /= size;
//	the_ultimate_stats->telescopes_used /= size;
//
//	the_ultimate_stats->opt_height_ratio /= size;
//	the_ultimate_stats->scheduled_above_min_height_ratio /= size;
//	the_ultimate_stats->scheduled_in_time_ratio /= size;
//	the_ultimate_stats->time_management_ratio /= size;
//
//	std::cout << "Valeurs moyennes des fonctions objectifs: " << std::endl;
//	std::cout << "Nombre de requêtes ordonnancées: " << the_ultimate_stats->scheduled_obs_ratio << std::endl;
//	std::cout << "Hauteur: " << the_ultimate_stats->height << std::endl;
//	std::cout << "Distance parcourue: " << the_ultimate_stats->distance << std::endl;
//	std::cout << "Nombre de télescopes: "<< the_ultimate_stats->telescopes_used << std::endl;
//
//	std::cout << endl;
//
//	std::cout << "Autres Stats: " << std::endl;
//	std::cout << "Nombre d'obs above min height: " << the_ultimate_stats->scheduled_above_min_height_ratio << std::endl;
//	std::cout << "Nombre d'obs in time: " << the_ultimate_stats->scheduled_in_time_ratio << std::endl;
//	std::cout << "Nombre d'obs au méridien " << the_ultimate_stats->opt_height_ratio << std::endl;
//	std::cout << "Exploitation du temps: "<< the_ultimate_stats->time_management_ratio << std::endl;
//
//}

