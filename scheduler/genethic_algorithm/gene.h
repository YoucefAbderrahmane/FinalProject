/*
x * gene.h
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#ifndef GENE_H_
#define GENE_H_

#include "../Model/Observation.h"

class gene {

	friend class chromosome;
	friend class population;
public:
	gene();
	gene(int index);
	virtual ~gene();

	bool is_scheduled(Observation observation);
	Observation * getObs();
	double get_start_time();
	double get_end_time();
	int is_in_time(Observation observation);
	int is_above_min_height(Observation observation);



	double getRandomSelection() const {
		return random_selection;
	}

	void setRandomSelection(double randomSelection) {
		random_selection = randomSelection;
	}

	double getStartDate() const {
		return start_date;
	}

	void setStartDate(double startDate) {
		start_date = startDate;
	}

	int getTelescopeUsed() const {
		return telescope_used;
	}

	void setTelescopeUsed(int telescopeUsed) {
		telescope_used = telescopeUsed;
	}

	int getIndex() const {
		return index;
	}

	void setIndex(int index) {
		this->index = index;
	}

	int getIsSched() const;
	void setIsSched(int isSched);

private:
	//Observation * observation; //the observation to be scheduled
	int index;

	double random_selection; /* Random value used to decide if the observation is selected to be observed or not
								by comparing it with the observation selection's threshold */
	double start_date; //in JD
	int telescope_used; //Id of the telescope used for observing the target

	int is_sched;

	int violated_const;

	double duration; 	//in seconds
	double end_date; //in JD


};

#endif /* GENE_H_ */
