/*
 * MyNSGA.cpp
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#include "MyNSGA.h"
#include <time.h>
#include "ga_config.h"

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

	//double tstart = clock();

	double k = (double) rand()/RAND_MAX;
	double x = 0.0;
	if(k < 0.75)
	{

		enf1->setObservations(p1.getObservations());
		enf2->setObservations(p2.getObservations());
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
			//std::cout<< "one point crossover"<< std::endl;
			enf1->genes.resize(size);
			enf2->genes.resize(size);
			int i = rand() % (size-1) + 1;//make sure to have at least one gene changed
			//std::cout<< i<< std::endl;
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
		if(Cross_uni /*1/3.0 <= x && x < 2/3.0*/)//uniform crossover
		{
			double u = 0.0 ;
			//std::cout<< "uniform crossover"<< std::endl;
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
		if( Cross_2/*2/3.0 <= x*/)//two points crossover
		{
			//std::cout<< "two points crossover"<< std::endl;
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
			//std::cout<< "taille de enf 1 est de "<< enf1->genes.size()<< std::endl;
			//std::cout<< "taille de enf 2 est de "<< enf2->genes.size()<< std::endl;
			//std::cout<< i << "  "<< j<< std::endl;
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

	//printf("CROSS Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

	}

void MyNSGA::mutation(chromosome *c, chromosome *cr, population p)
{

	//double tstart = clock();
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

		double start = Schedule::conditions->getNightHorizon().start;
		double sup = addSecondsToJD(Schedule::conditions->getNightHorizon().end, -Schedule::observations.at(i).getDuration());
		double h = sup - Schedule::conditions->getNightHorizon().start;

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
//			std::cout<< "mutation : N_t "<< N_t<< std::endl;
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

	//printf("MUTATION Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

}

void MyNSGA::nsga2(population *p)
{

	int NP = p->get_size();
	if(NP >= 5)
	{
	std::vector<int> v1(NP), v2(NP);
	chromosome parent1, parent2, enfant1, enfant2;
	for(int i=0;i < NP; i++)
	{
		v1.at(i) = i;
		v2.at(i) = i;
		//p->update_dom(i);
	}

	//std::cout << "nb gen " << nb_gen << std::endl;
	for(int g = 0; g< this->nb_gen; g++)
	{
		//clock_t tstart = clock();

		//std::cout << "gen num " << g << std::endl;

		p->update_pareto_information(); // update fronts and ranks


		//printf("START 1 Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

		//tstart = clock();


		//printf("START 2 Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
		//tstart = clock();

		population  cr(p->getIndividuals(),p->get_size(),p->n_telescopes); //Initialization de R

		//printf("START 3 Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
		//std::cout<<"_______________taille initial de R "<< g<< std::endl;

		//printf("START Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

		//clock_t s = clock();

		std::random_shuffle(v1.begin(),v1.end()); //shuffle the vectors to pick the parents randomly
		std::random_shuffle(v2.begin(),v2.end());

		//printf("START Time taken: %.5fs\n", (double)(clock() - s)/CLOCKS_PER_SEC);
		//std::cout<< "MyNSGA : start AG"<< std::endl;
		for(int i =0; i < NP; i+=4)
		{

			//clock_t start = clock();



			parent1 = tournamentSelection(v1[i],v1[i+1],*p);
			parent2 = tournamentSelection(v1[i+2],v1[i+3],*p);

			//std::cout << "pop " << p->individuals.at(0).getGenes().at(0).get_end_time() << std::endl;
			//std::cout << "v de i " << v1.at(0).individuals.at(0).getGenes().at(0).get_end_time() << std::endl;
		//	printf("SEL Time taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
			//start = clock();

			crossover(&enfant1,&enfant2,parent1,parent2);

			//printf("CROSS Time taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
			//start = clock();


			mutation(&enfant1, &enfant2, *p);
//			std::cout<< "MyNSGA : end of mutation 1"<< std::endl;

			//printf("MUT Time taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
			//start = clock();

			//std::cout << "one " << enfant1.getGenes().at(0).get_end_time() << std::endl;

			cr.repair(&enfant1);
//			std::cout<< "MyNSGA : end of repair 1"<< std::endl;
			cr.repair(&enfant2);
//			std::cout<< "MyNSGA : end of repair 2"<< std::endl;
			//printf("REP Time taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
			//start = clock();


			//maj des fitness contrainte et dom
			enfant1.compute_obj_func();
			cr.addIndividual(enfant1);
//			cr.update_dom(cr.get_size()-1);


			enfant2.compute_obj_func();
			cr.addIndividual(enfant2);
//			cr.update_dom(cr.get_size()-1);


			//printf("MAJ Time taken: %.5fs\n", (double)(clock() - start)/CLOCKS_PER_SEC);
//			start = clock();

			parent1 = tournamentSelection(v2[i],v2[i+1],*p);
			parent2 = tournamentSelection(v2[i+2],v2[i+3],*p);

			crossover(&enfant1,&enfant2,parent1,parent2);
			mutation(&enfant1, &enfant2, *p);
//			std::cout<< "MyNSGA : end of mutation 2"<< std::endl;
			cr.repair(&enfant1);
//			std::cout<< "MyNSGA : end of repair 3"<< std::endl;
			cr.repair(&enfant2);
//			std::cout<< "MyNSGA : end of repair 4"<< std::endl;
			enfant1.compute_obj_func();

			cr.addIndividual(enfant1);

//			cr.update_dom(cr.get_size()-1);


		    enfant2.compute_obj_func();

		    cr.addIndividual(enfant2);

//		    cr.update_dom(cr.get_size()-1);

		}

		for(int i = 0; i < 2*NP; i++)  cr.update_dom(i);

		//trier et choisir les meilleurs -> pareto information + tri + sélectionnner les NP premier

		cr.bestIndividuals(NP);
		//std::cout << "taille de la pop de cr ind size " << cr.individuals.size() << std::endl;
		//std::cout << "nombre de champion ind size " << cr.champions.size() << std::endl;

		p->individuals= cr.getChampions();
		//std::cout << "p ind size new " << p->individuals.size() << std::endl;

	//	std::cout<< "nombre de champions "<< cr.getChampions().size()<< std::endl;
		cr.champions.clear();
		//std::cout<< "MyNSGA : next generation"<< std::endl;
		for(int i = 0; i < NP; i++)  p->update_dom(i);

		//std::cout << "best done " << std::endl;
		//printf("GENERATION Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);

	}
	}
	else std::cout<< "not enough members in the population"<< std::endl;
}
