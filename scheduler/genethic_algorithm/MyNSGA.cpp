/*
 * MyNSGA.cpp
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#include "MyNSGA.h"

MyNSGA::MyNSGA() {
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
	if( /*k < 0.5*/1)
	{
		std::cout<< "two point crossover"<< std::endl;
		//std::cout<< k << std::endl;
		int size = p1.genes.size();
		x = (double) rand()/RAND_MAX;
		//std::cout<< x << std::endl;
		if(0/*x < 1/3.0*/){// one point crossover
			std::cout<< "one point crossover"<< std::endl;
			enf1->genes.resize(size);
			enf2->genes.resize(size);
			int i = rand() % (size-1) + 1;//make sure to have at least one gene changed
			std::cout<< i<< std::endl;
			//int k =0;
			std::copy(p1.genes.begin(),p1.genes.begin()+ i, enf1->genes.begin());
			std::copy(p2.genes.begin(),p2.genes.begin()+ i, enf2->genes.begin());
			/*for(k =0; k < i; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);

			}*/
			std::copy(p2.genes.begin()+i,p2.genes.end(), enf1->genes.begin() + i);
			std::copy(p1.genes.begin()+i,p1.genes.end(), enf2->genes.begin() + i);
			/*for(k=i; k< size; k++)
			{
				enf1->genes.push_back(p2.genes[k]);
				enf2->genes.push_back(p1.genes[k]);
			}*/
		}
		if(0 /*1/3.0 <= x && x < 2/3.0*/)//uniform crossover
		{
			double u = 0.0 ;
			std::cout<< "uniform crossover"<< std::endl;
			enf1->genes.clear();
			enf2->genes.clear();
			for(int i =0; i < size; i++)
			{
				u = (double) rand()/RAND_MAX;
				//std::cout<< "cout u each time "<< u<< std::endl;
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
		if( 1/*2/3.0 <= x*/)//two points crossover
		{
			//std::cout<< "two points crossover"<< std::endl;
			int i =0, j=0;
			while(i == j)
			{
			i = rand() % (size-2) + 1;
			j = i + rand() % (size-i);
			}
			enf1->genes.resize(size);
			enf2->genes.resize(size);
			std::cout<< i << "  "<< j<< std::endl;
			std::copy(p1.genes.begin(),p1.genes.begin()+ i, enf1->genes.begin());
			std::copy(p2.genes.begin(),p2.genes.begin()+ i, enf2->genes.begin());
			/*for(int k =0; k < i; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);
			}*/
			std::copy(p1.genes.begin() + i,p1.genes.begin()+ j, enf2->genes.begin() + i);
			std::copy(p2.genes.begin() + i,p2.genes.begin()+ j, enf1->genes.begin() + i);
			/*for(int k =i; k< j; k++)
			{
				enf1->genes.push_back(p2.genes[k]);
				enf2->genes.push_back(p1.genes[k]);

			}*/
			std::copy(p1.genes.begin() + j,p1.genes.end(), enf1->genes.begin() + j);
			std::copy(p2.genes.begin() + j,p2.genes.end(), enf2->genes.begin() + j);
			/*for(int k =j; k < size; k++)
			{
				enf1->genes.push_back(p1.genes[k]);
				enf2->genes.push_back(p2.genes[k]);

			}*/
		}
	}
	else {
		*enf1 = p1;
		*enf2 = p2;
	}
	}

void MyNSGA::mutation(chromosome *c, chromosome *cr, population p)
{
	double x = (double) rand()/RAND_MAX;
	if(x < 0.5)
	{
		std::cout<< "valeur ordo"<< std::endl;
		int size = c->genes.size();
		int i = rand() % size;
		std::cout<< size<< std::endl;
		std::cout<< "child one "<< c->genes[i].getRandomSelection();
		c->genes[i].setRandomSelection((double) rand()/RAND_MAX);
		std::cout<< "	new " << c->genes[i].getRandomSelection()<< endl;

		std::cout<< "child two "<< cr->genes[i].getRandomSelection();
		i = rand()% (c->genes.size());
		cr->genes[i].setRandomSelection((double) rand()/RAND_MAX);
		std::cout<< "	new " << cr->genes[i].getRandomSelection()<< endl;
	}
	x = (double) rand()/RAND_MAX;
	if(x < 0.5)
		{
		std::cout<<"début observ"<< std::endl;
		int size = c->genes.size();
		int i = rand()% size;

		double start = p.schedule.getConditions()->getNightHorizon().start;
		double h = p.schedule.getConditions()->getNightHorizon().end - p.schedule.getConditions()->getNightHorizon().start;
		double x = start + ((double) rand()/RAND_MAX) * h;


		c->genes[i].setStartDate(x);


		i = rand()% size;

		x = start + ((double)rand()/RAND_MAX) * h;
		cr->genes[i].setStartDate( x);

		}
	x = (double) rand()/RAND_MAX;
	if(x < 0.5)
		{
		std::cout<<	"tel alloué" << std::endl;
		int size = c->genes.size();
		int g = rand()% size;
			int i = rand() % N_TELESCOPE;

			std::cout<< "child one "<< g<< "  used telescope "<< c->genes[g].getTelescopeUsed();
			c->genes[g].setTelescopeUsed(i);
			std::cout<< "	new " << c->genes[g].getTelescopeUsed()<< endl;

			g = rand()% size;
			i = rand() % N_TELESCOPE;
			std::cout<< "child two "<< g<< "  used telescope "<< cr->genes[g].getTelescopeUsed();
			cr->genes[g].setTelescopeUsed(i);
			std::cout<< "	new " << cr->genes[g].getTelescopeUsed()<< endl;
		}
}

void MyNSGA::nsga2(population *p)
{
	int NP = p->get_size();
	std::vector<int> v1(NP), v2(NP);
	chromosome parent1, parent2, enfant1, enfant2;
	for(int i=0;i < NP; i++)
	{
		v1[i] = i;
		v2[i] = i;
		p->update_dom(i);
	}
	for(int g =0; g< this->nb_gen; g++)
	{
		p->update_pareto_information(); // update fronts and ranks
		p->updateViolation(); //update violation ratio of the constraints for the crossover
		population  cr(p->getIndividuals(),p->get_size()); //Initialization de R
		std::random_shuffle(v1.begin(),v1.end()); //shuffle the vectors to pick the parents randomly
		std::random_shuffle(v2.begin(),v2.end());
		//unsigned seed = std::

		for(int i =0; i < NP; i+=4)
		{
			parent1 = tournamentSelection(v1[i],v1[i+1],*p);
			parent2 = tournamentSelection(v1[i+2],v1[i+3],*p);
			crossover(&enfant1,&enfant2,parent1,parent2);
			mutation(&enfant1, &enfant2, *p);
			cr.repair(&enfant1);
			cr.repair(&enfant2);

			//maj des fitness contrainte et dom
			enfant1.compute_obj_func();
			cr.addIndividual(enfant1);
			cr.update_dom(cr.get_size()-1);

			enfant2.compute_obj_func();
			cr.addIndividual(enfant2);
			cr.update_dom(cr.get_size()-1);

			parent1 = tournamentSelection(v2[i],v2[i+1],*p);
			parent2 = tournamentSelection(v2[i+2],v2[i+3],*p);
			crossover(&enfant1,&enfant2,parent1,parent2);
			mutation(&enfant1, &enfant2, *p);
			cr.repair(&enfant1);
			cr.repair(&enfant2);
			//maj des fitness contrainte et dom
			enfant1.compute_obj_func();
			cr.addIndividual(enfant1);
			cr.update_dom(cr.get_size()-1);


			enfant2.compute_obj_func();
			cr.addIndividual(enfant2);
			cr.update_dom(cr.get_size()-1);

		}
		//trier et choisir les meilleurs -> pareto information + tri + sélectionnner les NP premier
		cr.bestIndividuals(NP);
		cr.clearChampions();
		p->setIndividuals(cr.getChampions());
		for(int i = 0; i < NP; i++) p->update_dom(i);
	}
}
