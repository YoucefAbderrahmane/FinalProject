/*
 * chromosome.h
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#ifndef CHROMOSOME_H_

#define CHROMOSOME_H_

#include "gene.h"
#include "ga_config.h"
#include "../Model/config.h"

class Observation;
class Target;

class chromosome {

	friend class population;
	friend class MyNSGA;

public:
	chromosome();
	chromosome(std::vector<Observation> Observations);
	virtual ~chromosome();

	void compute_constraints();
	void compute_obj_func();

	double get_obj_func(int index);
	double get_cst_val(int index);
	void clearDomList();
	void incrementDomCount();
	void decrementDomCount();

	void checkObservations();
	const std::vector<double>& getC() const {
			return c;
		}

		void setC(const std::vector<double>& c) {
			this->c = c;
		}

		double getCrowdingDist() const {
			return crowding_dist;
		}

		void setCrowdingDist(double crowdingDist) {
			crowding_dist = crowdingDist;
		}

		int getDomCount() const {
			return dom_count;
		}

		void setDomCount(int domCount) {
			this->dom_count = domCount;
		}

		std::vector<int> getDomList() {
			return dom_list;
		}

		void setDomList(const std::vector<int>& domList) {
			dom_list = domList;
		}

		const std::vector<double>& getF() const {
			return f;
		}

		void setF(const std::vector<double>& f) {
			this->f = f;
		}

		int getParetoRank() const {
			return pareto_rank;
		}

		void setParetoRank(int paretoRank) {
			pareto_rank = paretoRank;
		}

		double getDomListOf(int index);

	double getViolationRatio() const {
		return violation_ratio;
	}

	void setViolationRatio(double violationRatio) {
		violation_ratio = violationRatio;
	}
	void updateViolationRatio();

	std::vector<gene> getGenes() {
		return genes;
	}

	void setGenes(const std::vector<gene>& genes) {
		this->genes = genes;
	}
	gene getGene(int index);

	int is_close_to_meridien(int index);
	int is_above_min_height(int index);
	int is_away_moon(int index);
	int is_in_req_time(int index);

	void ratio_violated_const();

	double get_duration();
	int getNbMaxT();
	void setNbMaxT(int nbMaxT);

private:
//	std::vector<Observation> observations;
//	Obs_conditions conditions;

	std::vector<gene> genes;
	std::vector<int> telescopes_alloc;
	int nb_max_t; // number max of the telescopes allocated

	std::vector<double> f;	//objective functions vector
	std::vector<double> c; //constraints vector
	std::vector<int> overlap_index; //index of observations that overlaps with their upcoming observation

	int pareto_rank; //the Pareto non domination rank (Pareto fronts)
	double crowding_dist; //the Pareto crowding distance

	int dom_count; //the Pareto domination count for the individual
	std::vector<int> dom_list; //list of dominated individuals
	double violation_ratio;
};

#endif /* CHROMOSOME_H_ */
