/*
 * MyNSGA.h
 *
 *  Created on: 12 juil. 2016
 *      Author: root
 */

#ifndef MYNSGA_H_
#define MYNSGA_H_

#include "chromosome.h"
#include "population.h"
#include <algorithm>
#include "../utilities/time_calculation.h"
#include "ga_config.h"


class MyNSGA {
public:
	MyNSGA();
	MyNSGA(int nb_gen);
	virtual ~MyNSGA();
	chromosome tournamentSelection(int individu1, int individu2, population p);
	void mutation(chromosome *c, chromosome *cr, population p);
	void crossover(chromosome *enf1, chromosome *enf2, chromosome p1, chromosome p2);
	void nsga2(population *p);
	//chromosome tournamentSelection(int individu1, int individu2, population p);

private :
	int nb_gen;
	//double prob_cross;
	//double prob_mut;

};


#endif /* MYNSGA_H_ */
