/*
 * Telescope.cpp
 *
 *  Created on: 16 sept. 2016
 *      Author: root
 */

#include "Telescope.h"





Telescope::Telescope(): tobserv(),observed(),num(0), etat(2){
	// TODO Auto-generated constructor stub

}

Telescope::~Telescope() {
	// TODO Auto-generated destructor stub
}


void Telescope::executeSchedule() {
	int not_finished = tobserv.size()-1;
	time_t t;
	int i = 1;
	while(not_finished)
	{
		/*ln_get_timet_from_julian(this->tobserv.at(0).getStartDate(),&t);
		if( t - time(0) > 1)
			{
				this->etat = 2;
				sleep(t-time(0));
			}*/
		this->etat = 1;
		cout<< "exécution de l'observation "<< i << " sur le télescope "<< num +1 <<endl;
		sleep(5);
		cout<< "fin de l'exécution de l'observation "<< i << " sur le télescope "<< num +1  <<endl;
		observed.push_back(tobserv.at(0));
		tobserv.erase(tobserv.begin());
		not_finished --;
		i++;
		sleep(2);
	}
	this->etat = 0;

	}


int Telescope::getNum()  {
return num;
}

void Telescope::setNum(int num) {
this->num = num;
}

vector<gene> Telescope::getTobserv() {
return tobserv;
}

void Telescope::setTobserv( vector<gene> tobserv) {
this->tobserv = tobserv;
}

 vector<gene> Telescope::getObserved() {
	return observed;
}

void Telescope::setObserved( vector<gene> observed) {
	this->observed = observed;
}
