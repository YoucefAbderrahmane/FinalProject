//============================================================================
// Name        : scheduler.cpp
// Author      : YoucefKhaled
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdlib.h>
#include <vector>
#include <iterator>
#include <cmath>

#include <libnova/libnova.h>

#include "../Model/Observation.h"
#include "../Model/Schedule.h"
#include "../Model/Request.h"
#include "../Model/Target.h"

#include "../genethic_algorithm/chromosome.h"
#include "../genethic_algorithm/gene.h"
#include "../genethic_algorithm/population.h"
#include "../genethic_algorithm/MyNSGA.h"
#include <ctime>
#include <time.h>
#include <algorithm>
#include <limits>
#include "../utilities/time_calculation.h"
using namespace std;

/*struct obj_fct_comp {

	obj_fct_comp(int dim):obj_fct(dim) {};
	bool operator()(chromosome * c1, chromosome * c2){

			return (c1->get_obj_func(obj_fct) < c2->get_obj_func(obj_fct));
		}
int obj_fct;
};
struct crowding_d_comp {
	crowding_d_comp (population *p):ps(p){};
	bool operator()(int  c1, int  c2){

			return (ps->get_individual(c1).getCrowdingDist() < ps->get_individual(c2).getCrowdingDist());
		}
	population *ps;
};
int k;
int * get_inter()
{

	return &k;
}*/

int main() {

//	Target vega(279.2347, 38.7836);
//	Target kochab(222.676375, 74.1555);
//	ln_equ_posn * pos = new ln_equ_posn();
//	pos = vega.getEqCord();
//	cout << kochab.getAngularDistance(pos) << endl;

//
//	struct ln_rst_time * solar_rst = new ln_rst_time();
//
//	ln_lnlat_posn * observer = new ln_lnlat_posn();
//
//	observer->lat = LATITUDE;
//	observer->lng = LONGITUDE;
//
//	double JD = std::abs(2457641.04398) - 0.5;
//
//	ln_get_solar_rst(JD , observer, solar_rst);
//
//	std::cout << "lever soleil " << fixed << solar_rst->rise << std::endl;
//	std::cout << "coucher soleil "<< fixed << solar_rst->set << std::endl;
//
//
//
//
//	exit(0);

	Schedule * sched = new Schedule();

	sched->randomObservationListGenerator(200);

	cout << "Initializing the population..." << endl;

	population * p = new population(52, *sched); //population multiple of 4

	//std::cout << "pop " << fixed << p->getIndividualsPointer()->at(0).getGenes().at(0).get_start_time() << std::endl;

//	cout << "Initializing ended..." << endl;

//
//	cout << "Population initialized..." << endl;
//
	//p->check_init();
//
//	p->get_individual(0).compute_constraints();
//
//	cout << p->get_individual(0).get_cst_val(0);

//	sched->checkObservations();



	//chromosome * ch = new chromosome(*sched->getObs());

	//cout << "fknfi," << endl;



	//ch->compute_obj_func();
	/*for (int i = 0; i< p->get_size();i++) 	p->update_dom(i);
	p->update_pareto_information();
	p->updateViolation();*/
/*	for(int i =0; i < p->get_size(); i++)
	{
		cout<< "the violation ratio :"<< p->get_individual(i).getViolationRatio()<< endl;
	}*/


<<<<<<< HEAD
	MyNSGA algo(10);
=======
	MyNSGA algo(50);
>>>>>>> 0387e8b643872840f8158d247c8b573c76b05fbf
//	std::cout<< "algo init : end " << std::endl;





	/*chromosome par = algo.tournamentSelection(0,1,*p);
	chromosome par2 = algo.tournamentSelection(2,3, *p);
	chromosome e1, e2;*/

	//p->bestIndividuals(2);
	//vector<chromosome> cro = p->getIndividuals();


	//cout<< "le nombre de champions est de "<< cro.size()<< endl;

	/*for(int i = 0; i < 4; i++){

		p->repair(p->getIndividualPointeur(i));
	}*/


	/*for(int i = 0; i< sizec; i++) {
		cout<< fixed << p->get_individual(0).getGene(i).getStartDate() << endl;
	}*/
	population p2 = *p;
	population p3 = *p;
	clock_t tStart = clock();
	algo.nsga2(p);

	//std::cout<< "nsga : end " << std::endl;

	double end_t = (double)(clock() - tStart)/CLOCKS_PER_SEC;
	int size = (int) p->get_size();
	int sizec = p->get_individual(0).getGenes().size();

	cout<< "taille de la population "<< size<< endl;

	cout<< "nombre de genes " << sizec<< endl;

	p->show_stats();
	printf("\n Time taken full: %.2fs\n\n", end_t);

	cout<< "------------deuxième pop---------------"<< endl;

	MyNSGA algo2(50);
	clock_t tstart2 = clock();
	algo2.nsga2(&p2);

		//std::cout<< "nsga : end " << std::endl;

		double end_t2 = (double)(clock() - tstart2)/CLOCKS_PER_SEC;
		size = (int) p2.get_size();
		sizec = p2.get_individual(0).getGenes().size();

		cout<< "taille de la population "<< size<< endl;

		cout<< "nombre de genes " << sizec<< endl;

		p2.show_stats();
		printf("\n Time taken full 2: %.2fs\n\n", end_t2);


		cout<< "------------troisième pop---------------"<< endl;
	MyNSGA algo3(100);
		clock_t tstart3 = clock();

	algo3.nsga2(&p3);

		//std::cout<< "nsga : end " << std::endl;

		double end_t3 = (double)(clock() - tstart3)/CLOCKS_PER_SEC;
		size = (int) p3.get_size();
		sizec = p3.get_individual(0).getGenes().size();

		cout<< "taille de la population "<< size<< endl;

		cout<< "nombre de genes " << sizec<< endl;

		p3.show_stats();
		printf("\n Time taken full 3: %.2fs\n", end_t3);




//	std::cout << "Taux req " << p->getIndividualPointeur(0)->get_obj_func(0) << std::endl;
//	std::cout << "Hauteur moyenne " << p->getIndividualPointeur(0)->get_obj_func(1) << std::endl;
//	std::cout << "Distance moyenne " << p->getIndividualPointeur(0)->get_obj_func(2) << std::endl;
//	std::cout << "Télescopes " << p->getIndividualPointeur(8)->get_obj_func(3) << std::endl;
	//cout<< "aprs :"<< endl;
	/*for(int i = 0; i< sizec; i++)
		cout<< fixed <<p->get_individual(0).getGene(i).getStartDate() << endl;
	cout<< "taille de la population "<< size<< endl;*/




/*	// test AG
 * selection ok
 * crossover ok
 * mutation ok
	cout<< "begin crossover"<< endl;
	algo.crossover(&e1,&e2,par,par2);
	cout<< "par " << par.getGenes().size() << endl;
	cout<< "par2 " << par2.getGenes().size() << endl;
	cout<< "e1 " << e1.getGenes().size() << endl;
	cout<< "e2 " << e2.getGenes().size() << endl;


	for(int i = 0; i < (int) par.getGenes().size(); i++)
	{
		cout<<"numero du gene "<< i<< " :   ";
		cout<< fixed << par.getGene(i).getStartDate()<< "    "
				<< fixed << par2.getGene(i).getStartDate()<< "    "
				<< fixed << e1.getGene(i).getStartDate()<< "    "
				<< fixed << e2.getGene(i).getStartDate()<< endl;
	}
	algo.mutation(&e1,&e2, *p);
*/
	/*cout<< "the ranks of par :"<< par.getParetoRank()<< endl;
	cout<< "the crowding distance :"<< par.getCrowdingDist()<< endl;
	cout<< "the violation ratio :"<< par.getViolationRatio()<< endl;*/

	/*test si shuffle de deux vecteurs successifs est différent
	vector<int> v1, v2;
	for(int i =0; i < 10; i++)
		{v1.push_back(i);
		v2.push_back(i);
		}
	random_shuffle(v1.begin(),v1.end());
	random_shuffle(v2.begin(),v2.end());
	for(vector<int>::iterator i = v1.begin(); i < v1.end(); i++)
	{
		cout<< *i << endl;
	}
	cout<< "_________________________________"<< endl;
	for(vector<int>::iterator i = v2.begin(); i < v2.end(); i++)
		{
			cout<< *i << endl;
		}*/
//test pareto
	/*std::vector<chromosome *>  front;
	std::vector<chromosome *> point;
	//front = p->getIndividualsPointer();
	for(int i =0; i < p->getIndividuals().size(); i++)
	{
		front.push_back(p->getIndividualPointeur(i));

	}
	cout<< "taille de pop "<< p->get_size()<< endl;
	//for (int i = 0; i< p->get_size();i++) 	p->update_dom(i);
	p->update_pareto_information();
	for(int i=0; i < p->get_size();i++) cout<< " "<< p->get_individual(i).getDomCount();
	cout<< "\naffichage des fronts";
	p->displayFronts();
	for(int y =0; y < front.size(); y++)
		{
			cout<<"\nI am individual "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " "<< front[y]->get_obj_func(1)<< " "<< front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3)<<endl;
		}
	vector<double> ideal = p->compute_nadir();
	cout<< "taille de ideal dans pop "<< ideal.size()<< endl;
	for(int i =0; i< PROB_DIM; i++)
	{
		cout<<ideal[i]<< "  ";
	}
	cout<< endl;
	cout<< "taille de pop	"<< front.size()<< endl;
	for(int i=0; i< p->get_size(); i++)
	{
		cout<< "individu "<< i << " domination cout = "<< p->get_individual(i).getDomCount()<< endl;
	}

	for (int i = 0; i< p->get_size();i++) 	p->update_dom(i);
	p->update_pareto_information();
	cout<< "_______________________________________________________________________________________"<< endl;
	for(int j =1; j < p->get_size();j++)
	{
		int k = p->compare_fitness(  p->get_individual(0).getF(), p->get_individual(j).getF()    );
		cout<< "compare fitness de 0 avec "<< j<< " donne "<< k << endl;
	}
	for(int i=0; i< p->get_size(); i++)
		{
			cout<< "individu "<< i << " domination cout = "<< p->get_individual(i).getDomCount()<< endl;
		}
	cout<< "_______________________________________________________________________________________"<< endl;
	p->displayFronts();*/

	//test crowding_distance
/*	p->update_crowding_dist(front);
	for(int i =0; i <p->getIndividuals().size(); i++)
	{
		cout << "crowding distance de  " << i << "	" << p->get_individual(i).getCrowdingDist()<< endl;
	}
	crowding_d_comp c(p);
	vector<int> vv;


		vv = p->getFronts()[0];
		sort(vv.begin(),vv.end(),c);
		for(int j =0; j < vv.size(); j++)
		{
		cout<< "nombre d'individus dans le fronts est : "<< p->get_individual(vv[j]).getCrowdingDist()<< endl;
		}*/

// test de f.obj

	/*cout<< f.obj_fct<<endl;
	cout <<"si obj_fct égale 0 :"<< front[0]->get_obj_func(f.obj_fct) << "	"<< front[0]->get_obj_func(0)<<endl;
	cout<<"_______________________________"<<endl;

	for(int y =0; y < front.size(); y++)
	{
		cout<<"I am individual "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " "<< front[y]->get_obj_func(1)<< " "<< front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3)<<endl;
	}

	//f.obj_fct++;
	sort(front.begin(),front.end(),f);
	for(int y =0; y < front.size(); y++)
		{
		cout<<"I am individual new "<< y << " and here are my component:"<< front[y]->get_obj_func(0)<< " " << front[y]->get_obj_func(1)<< " "<<front[y]->get_obj_func(2)<< " "<< front[y]->get_obj_func(3) <<endl;
		}*/
// test de update dom
/*	for(int i =0; i < (*front).size(); i++)
	{
		point.push_back(&(*front)[i]);
		cout<<"je suis l'individu "<< i<< "		";
		p->update_dom(i); //ok
		cout<< endl;
	}


	cout<< "dom count "<< point[0]->getDomCount()<< " front aussi "<< (*front)[0].getDomCount()<<endl;
		cout<< "dom count "<< point[1]->getDomCount()<< " front aussi "<< (*front)[1].getDomCount()<<endl;
		cout<< "dom count "<< point[2]->getDomCount()<< " front aussi "<< (*front)[2].getDomCount()<<endl;
	cout<< "___________________"<< endl;
	//p->update_crowding_dist(point); // ok
	for(int i =0; i < (*front).size(); i++)
	{
		point.push_back(&(*front)[i]);
		cout<<"je suis l'individu "<< i<< "		";
		p->update_dom(i); //ok
		cout<< endl;
	}

	cout<< "dom count "<< point[0]->getDomCount()<< " front aussi "<< (*front)[0].getDomCount()<<endl;
	cout<< "dom count "<< point[1]->getDomCount()<< " front aussi "<< (*front)[1].getDomCount()<<endl;
	cout<< "dom count "<< point[2]->getDomCount()<< " front aussi "<< (*front)[2].getDomCount()<<endl;*/
	//cout<< "premier élément de dom_list de 0 "<< point[0]->getDomList()[0]<< endl;
//test fonction pareto
	/*
	//front[1].setDomCount(2); ok
	//cout << front[1].getDomCount(); ok
	//front[1].incrementDomCount(); ok
	//front[1].decrementDomCount(); ok
	//p->update_dom(i); //ok
	//obj_fct_comp f(0); ok
	//p->update_crowding_dist(front); ok
	//p->update_pareto_information(); ok*/

	//various tests
		/*/(*front)[0].setDomCount(1500);

	//p->displayFronts();

			//p->getIndividualPointeur(0)->setDomCount(1500);

			//(p->getIndividualPointeur(0))->setCrowdingDist(120);

			//std::vector<chromosome>::iterator i = p->getIndividuals().begin();
			for(std::vector<chromosome>::iterator i = p->getIndividuals().begin(); i < p->getIndividuals().end(); i++)
			{
				front.insert(i,p->get_individual(i));
			}*/

//	Target RigilKentaurus(219.8795833, -60.81952778);
//
//	Target * Aldebaran = new Target(68.98041667, 16.50841667);


//	vector<Observation> o;
//	double scheduled;
//	double fraction = 0.0;
//	double moyenne =0.0;
//	for(int j =0 ; j< size; j++)
//	{
//	o = p->get_individual(j).getObservations();
//	scheduled = 0.0;
//	for(int i = 0; i < sizec; i++)
//	{
//		scheduled += (int) p->get_individual(j).getGenes().at(i).is_scheduled(o[i]); //o[i].getTaken();
//	}
//
//	fraction = scheduled/ sizec;
//	moyenne += fraction;
//	cout<< "le pourcentage de requêtes ordonnancées de l'individu "<< j<< " est de :"<< fraction* 100<< endl;
//	}
//	cout<< "moyenne des requêtes ordonnancées dans la population "<< moyenne/size*100<< endl;
//	MyNSGA algo2(50);
//	algo2.nsga2(&p2);
//	moyenne = 0.0;
//	for(int j =0 ; j< size; j++)
//	{
//	o = p->get_individual(j).getObservations();
//	scheduled = 0.0;
//	for(int i = 0; i < sizec; i++)
//	{
//		scheduled += (int) p2.get_individual(j).getGenes().at(i).is_scheduled(o[i]); //o[i].getTaken();
//	}
//
//	fraction = scheduled/ sizec;
//	moyenne += fraction;
//	cout<< "le pourcentage de requêtes ordonnancées de l'individu "<< j<< " est de :"<< fraction* 100<< endl;
//	}
//	cout<< "moyenne des requêtes ordonnancées dans la population "<< moyenne/size*100<< endl;

	return 0;
}

