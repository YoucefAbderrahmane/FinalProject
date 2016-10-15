/*
 * Telescope.h
 *
 *  Created on: 16 sept. 2016
 *      Author: root
 */

#ifndef TELESCOPE_H_
#define TELESCOPE_H_

#include "chromosome.h"
#include "gene.h"
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <ctime>
#include <libnova/libnova.h>


using namespace std;

class Telescope {
	friend class population;
public:
	Telescope();
	virtual ~Telescope();

	void executeSchedule();
	int getNum() ;
	void setNum(int num);
	vector<gene> getTobserv();
	void setTobserv( vector<gene> tobserv);
	vector<gene> getObserved() ;
	void setObserved( vector<gene> observed);

private:
	vector<gene> tobserv;
	vector<gene> observed;
	int num;
	int etat; // 0 nothing to do, 1 executing observation, 2 waiting for the next observation
};



#endif /* TELESCOPE_H_ */
