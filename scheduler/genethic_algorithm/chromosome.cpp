/*
 * chromosome.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "chromosome.h"
#include <cmath>

chromosome::chromosome() : observations(), genes(), telescopes_alloc(), f(), c(), overlap_index(), pareto_rank(), crowding_dist(),
		dom_count(), dom_list() {
	// TODO Auto-generated constructor stub

	f.reserve(PROB_DIM);
	c.reserve(CSTR_DIM);
	telescopes_alloc.reserve(N_TELESCOPE);
}

chromosome::chromosome(std::vector<Observation> * observations) : observations(), genes(), telescopes_alloc(), f(), c(),
		overlap_index(), pareto_rank(), crowding_dist(), dom_count(), dom_list(){

	this->observations = observations;

	f.resize(PROB_DIM);
	c.resize(CSTR_DIM);
	telescopes_alloc.resize(N_TELESCOPE);
	overlap_index.resize(observations->size());

	gene * g;
	for(std::vector<Observation>::size_type i = 0; i < observations->size(); i++){

		g = new gene();
		//g->observation = &observations->at(i);
		genes.push_back(*g);
	}

}

chromosome::~chromosome() {
	// TODO Auto-generated destructor stub
}


void chromosome::compute_obj_func() {


	f[0] = 0.0; //f[0] : number of scheduled observations (maximize this value)
	f[1] = 0.0; //f[1] : Average of altitude merits of all observed targets (maximize)
	f[2] = 0.0; //f[2] : Average of telescope movement during the whole schedule (minimize)
	f[3] = 0.0; //f[3] : number of telescopes used for scheduling (minimize)

	std::vector<double> telescope_dist;
	telescope_dist.resize(N_TELESCOPE);

	std::vector<Observation> last_teles_obs;
	last_teles_obs.resize(N_TELESCOPE);

	std::vector<int> first(N_TELESCOPE, 0);
	//first.reserve(N_TELESCOPE);

	//	std::vector<double> telescope_dist(N_TELESCOPE, 0); //contains distances traveled by each telescope
//	std::vector<Observation> last_teles_obs(N_TELESCOPE, 0); //contains the last observation visited by the telescope
//	std::vector<bool> first(N_TELESCOPE, true); //helping variable for computing the movement of one telescope

	//vars
	ln_equ_posn * pos;
	int i = 0;
	int curr_teles;
	double dist;

	//range all the observations vector
	for(i = 0; i < (int) genes.size(); i++){

		//if the observation is scheduled
		if( genes.at(i).is_scheduled(observations->at(i)) ){

			curr_teles = genes.at(i).telescope_used;	//get the telescope id
			telescopes_alloc[curr_teles] = 1;	//mark the telescope as used


			f[0]++;	//increase the number of scheduled observations

			//sum altitude merits of all observed targets
			//f[1] += genes.at(i).observation->altituteMerit();
			f[1] += observations->at(i).altituteMerit();

			//computing the distance traveled by each telescope

			if(first[curr_teles] == 0){ //if it is the first observation, save it and move on

				first[curr_teles] = 1;
			}
			else{ //if not, compute the distance with the last observation

				//compute the actual distance traveled
				pos = new ln_equ_posn();
				//pos = genes.at(i).observation->getTarget().getEqCord();
				pos = observations->at(i).getTarget().getEqCord();
				dist = last_teles_obs[curr_teles].getTarget().getAngularDistance(pos);
				telescope_dist[curr_teles] += dist;
			}

			//update the last observation visited of the current telescope
			last_teles_obs[curr_teles] = observations->at(i);//* genes.at(i).observation;
		}
	}

	//get the average altitude merit
	f[1] = f[1] / f[0];

	//get the average distance traveled
	//AND
	//the number of telescopes used
	for(i = 0; i < N_TELESCOPE; i++){

		//Avg dist
		f[2] += telescope_dist[i] / N_TELESCOPE;

		//Num teles
		f[3] += telescopes_alloc[i];
	}

	//Converting maximization problems into minimization problems (default)
	f[1] = -f[1];
	f[2] = -f[2];
}

double chromosome::get_obj_func(int index) {

	return f[index];
}


struct gene_less_comp{

	bool operator()(gene g1, gene g2){

		return (g1.get_start_time() < g2.get_start_time());
	}
};

void chromosome::compute_constraints() {

	c[0] = 1.0;

	//Check if there is any overlapping between two consecutive observations
	//If there is an overlap, set the constraint vector's first value to 1, otherwise, set it to 0


	//YOU MUST SORT ALL OBSERVATIONS AND TAKE CARE THAT YOU COMPARE OBSERVATIONS OF THE SAME TELESCOPE



	//copy the observation vector and sort it using std::sort and the struct above
	std::vector<gene> genes_copy = genes;

	//sort the copied vector
	std::sort(genes_copy.begin(), genes_copy.end(), gene_less_comp());

	//use it to detect overlapping between observations of the same telescope

	//std::vector<gene> curr;
	//curr.resize(N_TELESCOPE);
	gene curr;
	//std::vector<int> found(N_TELESCOPE, 0);
	int found = 0;
	for(int t = 0; t < N_TELESCOPE; t++){

		found = 0;
		for(std::vector<gene>::size_type i = 0; i < genes_copy.size() - 1; i++){

			if (genes_copy[i].telescope_used == t) {

				if( found == 0 ){

					curr = genes_copy[i];
					found = 1;
				}
				else{

					if( curr.get_end_time() >= genes_copy[i].get_start_time() ){

						overlap_index[i] = 1;
						c[0] = 1.0;
					}
					curr = genes_copy[i];
				}
			}
		}
	}

	std::cout << c[0] << std::endl;
}

double chromosome::get_cst_val(int index) {

	return c[index];
}

//void check_constraints(){
//
//
//}


void chromosome::checkObservations() {

	//Use this function to check if observations are generated correctly
	//This function will become useless when the number of observation is too high

	std::cout << "Number of generated observations : " << genes.size() << std::endl;

	for(int i = 0; i < (int) genes.size(); i++){

		std::cout << "Observation " << i << std::endl;
		std::cout << "Obs req id : " << observations->at(i).getRequest()->getReqId() << std::endl;
		std::cout << "Obs id : " << observations->at(i).getObsId()<< std::endl;
		std::cout << "Obs exp time : " << fixed << observations->at(i).getExposureTime() << std::endl;
		std::cout << "Obs min h : " << fixed << observations->at(i).getMinHeight() << std::endl;
		std::cout << "Obs min moon : " << fixed << observations->at(i).getMoonMinSeparation() << std::endl;
		std::cout << "Obs req time : " << fixed << observations->at(i).getReqTime().start << " - "
				<< fixed << observations->at(i).getReqTime().end << std::endl;
		std::cout << "Obs target : " << "dec " << fixed << observations->at(i).getTarget().getEqDec() << " - "
				<< "ra " << fixed << observations->at(i).getTarget().getEqRAsc() << std::endl;
		std::cout << endl;
	}
}
