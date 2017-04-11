/*
 * MyNSGA.cpp
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#include "MyNSGA.h"
#include <time.h>
#include "ga_config.h"
#include "../Model/Obsconditions.h"

MyNSGA::MyNSGA():nb_gen() {
	// TODO Auto-generated constructor stub

}

MyNSGA::MyNSGA(int nb_gen):nb_gen(nb_gen) {
	// TODO Auto-generated constructor stub

}


MyNSGA::~MyNSGA() {
	// TODO Auto-generated destructor stub
}


chromosome MyNSGA::tournamentSelection(int individu1, int individu2, population p)
{
	if(p.get_individual(individu1).pareto_rank < p.get_individual(individu2).pareto_rank) return p.get_individual(individu1);
	if(p.get_individual(individu1).pareto_rank > p.get_individual(individu2).pareto_rank) return p.get_individual(individu2);
	if(p.get_individual(individu1).crowding_dist > p.get_individual(individu2).crowding_dist) return p.get_individual(individu1);
	if(p.get_individual(individu1).crowding_dist < p.get_individual(individu2).crowding_dist) return p.get_individual(individu2);
	if(p.get_individual(individu1).violation_ratio < p.get_individual(individu2).violation_ratio) return p.get_individual(individu1);
	if(p.get_individual(individu1).violation_ratio > p.get_individual(individu2).violation_ratio) return p.get_individual(individu2);
	return ((rand()/RAND_MAX) < 0.5 ?  p.get_individual(individu1) : p.get_individual(individu2));
}

void MyNSGA::crossover(chromosome *enf1, chromosome *enf2, chromosome p1, chromosome p2)
{

	double k = (double) rand()/RAND_MAX;
	double x = 0.0;
	if(k < 0.75)
	{

//		enf1->setObservations(p1.getObservations());
//		enf2->setObservations(p2.getObservations());
		int size = p1.genes.size();
		x = (double) rand()/RAND_MAX;

		if(x < 0.5)
		{
			enf1->setNbMaxT(p1.getNbMaxT());
			enf2->setNbMaxT(p2.getNbMaxT());
		}
		else
		{
			enf1->setNbMaxT(p2.getNbMaxT());
			enf2->setNbMaxT(p1.getNbMaxT());
		}

		if(Cross_1/*x < 1/3.0*/){// one point crossover

			enf1->genes.resize(size);
			enf2->genes.resize(size);
			int i = rand() % (size-1) + 1;//make sure to have at least one gene changed

			std::copy(p1.genes.begin(),p1.genes.begin()+ i, enf1->genes.begin());
			std::copy(p2.genes.begin(),p2.genes.begin()+ i, enf2->genes.begin());

			std::copy(p2.genes.begin()+i,p2.genes.end(), enf1->genes.begin() + i);
			std::copy(p1.genes.begin()+i,p1.genes.end(), enf2->genes.begin() + i);
		}

		if(Cross_uni /*1/3.0 <= x && x < 2/3.0*/)//uniform crossover
		{
			double u = 0.0 ;
			enf1->genes.clear();
			enf2->genes.clear();
			for(int i =0; i < size; i++)
			{
				u = (double) rand()/RAND_MAX;
				if( u < 0.5)
				{
					enf1->genes.push_back(p1.genes[i]);
					enf2->genes.push_back(p2.genes[i]);
				}
				else {
					enf1->genes.push_back(p2.genes[i]);
					enf2->genes.push_back(p1.genes[i]);
				}
			}
		}

		if( Cross_2/*2/3.0 <= x*/)//two points crossover
		{
			int i =0, j=0;
			while(i == j)
			{
				i = rand() % (size-2) + 1;
				j = i + rand() % (size-i);
			}
			enf1->genes.clear();
			enf2->genes.clear();
			enf1->genes.resize(size);
			enf2->genes.resize(size);

			std::copy(p1.genes.begin(),p1.genes.begin()+ i, enf1->genes.begin());
			std::copy(p2.genes.begin(),p2.genes.begin()+ i, enf2->genes.begin());
			std::copy(p1.genes.begin() + i,p1.genes.begin()+ j, enf2->genes.begin() + i);
			std::copy(p2.genes.begin() + i,p2.genes.begin()+ j, enf1->genes.begin() + i);

			std::copy(p1.genes.begin() + j,p1.genes.end(), enf1->genes.begin() + j);
			std::copy(p2.genes.begin() + j,p2.genes.end(), enf2->genes.begin() + j);

		}
	}
	else {
		*enf1 = p1;
		*enf2 = p2;
	}

	//printf("CROSS Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

	}

void MyNSGA::mutation(chromosome *c, chromosome *cr, population p)
{

	Obs_conditions * conditions = new Obs_conditions();

	int size = c->genes.size();
	int i = rand()% size;
	double j = rand()% size;

	double x = (double) rand()/RAND_MAX;
	if(x < 0.2){

		if( c->genes.at(i).getIsSched() ) c->genes.at(i).setIsSched(0);
		else c->genes.at(i).setIsSched(1);

		if( cr->genes.at(i).getIsSched() ) cr->genes.at(i).setIsSched(0);
		else cr->genes.at(i).setIsSched(1);
	}

	x = (double) rand()/RAND_MAX;
	if(x < 0.8){

		double start = conditions->night_horizon.start;
		double sup = addSecondsToJD(conditions->night_horizon.end, -conditions->observations.at(i).getDuration());
		double h = sup - conditions->night_horizon.start;

		double x = start + ((double) rand()/RAND_MAX) * h;
		c->genes[i].setStartDate(x);

		x = start + ((double)rand()/RAND_MAX) * h;
		cr->genes[j].setStartDate( x);
	}

	x = (double) rand()/RAND_MAX;

	if(x < 0.8){
		int t = 0;
		//c->setNbMaxT((rand()%N_TELESCOPE)+1);
		int N_t = c->getNbMaxT();

		x = (double) rand()/RAND_MAX;
		if(x < 0.8){

			if (c->genes[i].getTelescopeUsed() > 0) t = c->genes[i].getTelescopeUsed()-1;
			else t = rand() % N_t;

			c->genes[i].setTelescopeUsed(t);
		}
		else{

			t = rand() % N_t;
			c->genes[i].setTelescopeUsed(t);
		}

		x = (double) rand()/RAND_MAX;
		if(x < 0.8){

			if (cr->genes[i].getTelescopeUsed() > 0) t = cr->genes[i].getTelescopeUsed()-1;
			else t = rand() % N_t;

			cr->genes[i].setTelescopeUsed(t);
		}
		else{

			t = rand() % N_t;
			cr->genes[i].setTelescopeUsed(t);
		}
	}
}


population * MyNSGA::make_offspring(population * p){

	//I- Variables setting and initialization
	int np = p->individuals.size();
	population * Q = new population(np, N_TELESCOPE);

	chromosome parent1, parent2, enfant1, enfant2; //parents and children, for genetic operations
	std::vector<int> v1(np), v2(np); //two vectors used for the selection process

	if( np < 5 ){

		cout << "The population size must be larger than 5" << endl;
		exit(0);
	}
	else{

		for(int i=0;i < np; i++)
		{
			v1.at(i) = i;
			v2.at(i) = i;
		}
	}

	//II- Creation of the initial offspring population Q, then combining it with population P

	//1- Selection
	//2- Crossover
	//3- Mutation
	//4- Merge P and Q into R

	std::random_shuffle(v1.begin(),v1.end()); //shuffle the vectors to pick the parents randomly
	std::random_shuffle(v2.begin(),v2.end());

	int i=0;
	while( (int) (Q->individuals.size()) < np && i < v1.size() ){

		parent1 = tournamentSelection(v1[i],v1[i+1],*p);
		parent2 = tournamentSelection(v1[i+2],v1[i+3],*p);

		crossover(&enfant1,&enfant2,parent1,parent2);
		mutation(&enfant1, &enfant2, *p);
//		Q->repair(&enfant1);
//		Q->repair(&enfant2);

		//maj des fitness contrainte et dom
		enfant1.compute_obj_func();
//		Q->addIndividual(enfant1);
		Q->individuals.push_back(enfant1);

		if( Q->individuals.size() == np) break;

		enfant2.compute_obj_func();
//		Q->addIndividual(enfant2);
		Q->individuals.push_back(enfant2);

		if( Q->individuals.size() == np) break;

		parent1 = tournamentSelection(v2[i],v2[i+1],*p);
		parent2 = tournamentSelection(v2[i+2],v2[i+3],*p);

		crossover(&enfant1,&enfant2,parent1,parent2);
		mutation(&enfant1, &enfant2, *p);
//		Q->repair(&enfant1);
//		Q->repair(&enfant2);

		enfant1.compute_obj_func();
//		Q->addIndividual(enfant1);
		Q->individuals.push_back(enfant1);

		if( Q->individuals.size() == np) break;

		enfant2.compute_obj_func();
//		Q->addIndividual(enfant2);
		Q->individuals.push_back(enfant2);

		i = i+4;
	}

	return Q;
}

struct crw_dist_comp {

	bool operator()(chromosome * c1, chromosome * c2){

			return (c1->getCrowdingDist() > c2->getCrowdingDist());
		}
};

void MyNSGA::nsga2(population *p)
{
	//I- Variables and environment initialization

		int np = p->get_size(); //population size
		if( np < 5 ){
			cout << "The population size must be larger than 5" << endl;
			exit(0);
		}

	//II- Creation of the initial offspring population Q, then combining it with population P
		population * Q = make_offspring(p); //Make a new population using the Selection, Crossover, Mutation

//MAIN LOOP BEGIN
	for(int g = 0; g< this->nb_gen; g++)
	{
		Q->individuals.insert(Q->individuals.end(), p->individuals.begin(), p->individuals.end());
		//Q now represents R the union of P and Q

	//III- Fast non dominated sort (fronts construction)
		Q->update_pareto_information();

	//IV- Main loop (generations)
		//0- Reset the p set of individuals to zero
			//p = new population(np, N_TELESCOPE);
			p->individuals.clear();
			p->individuals.reserve(np);

		//1- While the population size has not been reached
			int i = 0;

			while( (p->individuals.size() + Q->fronts.at(i).size()) < np ){
				//a- Crowding distance assignment (calculation)
				std::vector<chromosome *> front = Q->update_crowding_dist(Q->fronts.at(i));
				//b- P and F (front) union
				for(int k=0; k < front.size(); k++){
					p->individuals.push_back(*front.at(k));
				}
				i++;
			}

		//2- Sort the next front F using the crowding distance
			//crw_dist_comp comp();
			std::vector<chromosome *> front = Q->update_crowding_dist(Q->fronts.at(i));
			std::sort(front.begin(),front.end(),crw_dist_comp());

		//3- Assign the rest of the individuals from the front to P
			i = 0;
			while(p->individuals.size() < np){
				p->individuals.push_back(*front.at(i));
				i++;
			}

		//4- Make the new offspring population Q
			Q->individuals.clear();
			Q = make_offspring(p);
	}
}
