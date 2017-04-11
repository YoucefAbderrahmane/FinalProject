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
#include <unistd.h>

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
#include <pthread.h>
#include "../utilities/time_calculation.h"
using namespace std;


std::vector<pthread_t> threads;
vector<Telescope> telescopes;
chromosome ordo;
struct sched_i{
	sched_i(population *pop, Schedule *schedu):p(pop),sched(schedu){}
	population *p;
	Schedule * sched;
};

void * ExecuteTelescope(void *telescope)
{
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,NULL);
    Telescope * tel = (Telescope *) telescope;
    tel->executeSchedule();
    pthread_exit(0);

}

void * Inter(void *i) //ajouter schedule et population en enrée
{
	sched_i *sc = (sched_i *)i;
	cout<< "lancement du thread interruption"<< endl;
	double interruption = (double) rand()/RAND_MAX;
	int nbt;
	cout << "taille dans le thread interruption "<< sc->p->get_size() << endl;
	sleep(20);
	//if(interruption < 0.05)
	//{
		nbt = rand()% (N_TELESCOPE-1);
	//	if(nbt > N_TELESCOPE - (int) threads.size())
		//{
			int size = (int) threads.size();
			int sizeg=0;
			vector<Observation>::iterator beg = Schedule::observations.begin();
			vector<gene> g;
			vector<chromosome> *individuals  = sc->p->getIndividualsPointer();
			int sizei = (int) individuals->size();
			for(int i = 0; i < size;i++)// stop the telescope i
			{

				pthread_cancel(threads.at(i));
				sizeg = (int) telescopes.at(i).getObserved().size();
				g = telescopes.at(i).getObserved();
				for(int j =0; j < sizeg; j++)// all what has been observed by telescope i
				{
					//update the observations states
					Schedule::observations.erase(beg + g.at(j).getIndex());
				}
			}
			cout<< "exécution de l'interruption"<< endl;
			sleep(5);
			cout<< "fin de l'exécution de l'interruption"<< endl;
				sc->p = new population(12,*(sc->sched), N_TELESCOPE -nbt +1);
				MyNSGA algo(10);
				algo.nsga2(sc->p);
				sc->p->setSolution();
				int rc=0;
				ordo = sc->p->getOrdo();
				int sizet = ordo.getNbMaxT();
				telescopes.resize(sizet);
				for(int i=0; i < sizet; i++) telescopes.at(i).setNum(i);
				sc->p->RetreiveRequestTelescopes(&telescopes);
				threads.resize(sizet);
				Telescope *tel;
				pthread_attr_t attr;
				pthread_attr_init(&attr);
				pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
				for(int i =0; i < sizet; i++)
				{
					tel = &telescopes.at(i);
					rc = pthread_create(&threads.at(i),&attr,ExecuteTelescope,(void*) tel);
				}
				pthread_exit(0);
			}

		//}
	//}


int main() {




/* former code*/

	Schedule * sched = new Schedule();

	sched->randomObservationListGenerator(200);

	cout << "Initializing the population..." << endl;

	population * p = new population(52, *sched, N_TELESCOPE); //population multiple of 4

	MyNSGA algo(50);

	clock_t tStart = clock();
	algo.nsga2(p);
	printf("\n Time taken full: %.2fs\n\n", (double)(clock() - tStart)/CLOCKS_PER_SEC);
	int size = (int) p->get_size();
	int sizec = p->get_individual(0).getGenes().size();

	cout<< "taille de la population "<< size<< endl;

	cout<< "nombre de genes " << sizec<< endl;

	//p->show_stats();

//
//	int rc=0;
//	ordo = p->getOrdo();
//	int sizet = ordo.getNbMaxT();
//	telescopes.resize(sizet);
//	for(int i=0; i < sizet; i++) telescopes.at(i).setNum(i);
//	p->RetreiveRequestTelescopes(&telescopes);
//	threads.resize(sizet);
//	Telescope *tel;
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//	for(int i =0; i < sizet; i++)
//	{
//		tel = &telescopes.at(i);
//		rc = pthread_create(&threads.at(i),&attr,ExecuteTelescope,(void*) tel);
//		if(rc) cout<< "impossible de lancer le thread"<< endl;
//	}
//	void * status;
//
//	pthread_t interuption;
//	sched_i sc(p,sched);
//	cout<< "lancement de l'interruption"<< endl;
//	rc = pthread_create(&interuption, &attr,Inter,(void *) &sc );
//	if(rc) cout<< "impossible de lancer l'interruption"<< endl;
//	for(int i = 0; i < sizet; i++)
//	{
//	pthread_join(threads.at(i), &status);
//	}
//	pthread_exit(0);
//	return 0;

	//	Target vega(279.2347, 38.7836);
	//	Target kochab(222.676375, 74.1555);
	//	ln_equ_posn * pos = new ln_equ_posn();
	//	struct ln_rst_time * solar_rst = new ln_rst_time();
	//	ln_lnlat_posn * observer = new ln_lnlat_posn();

}

