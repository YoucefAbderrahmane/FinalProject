/*
 * chromosome.cpp
 *
 *  Created on: 16 juil. 2016
 *      Author: root
 */

#include "chromosome.h"
#include <cmath>
#include <time.h>
#include <algorithm>
#include <libnova/libnova.h>
#include "../utilities/time_calculation.h"
#include "../Model/Schedule.h"

chromosome::chromosome() : observations(), genes(), telescopes_alloc(), violation_ratio(0), f(), c(), overlap_index(), pareto_rank(), crowding_dist(),
		dom_count(), dom_list(), nb_max_t(0) {
	// TODO Auto-generated constructor stub

	f.resize(PROB_DIM);
	c.resize(CSTR_DIM);
	telescopes_alloc.resize(N_TELESCOPE);

	//std::cout << "instantiation succeeded." << std::endl;
}

chromosome::chromosome(std::vector<Observation> observations) : observations(), genes(), telescopes_alloc(),
		violation_ratio(0.0), f(), c(),
		overlap_index(), pareto_rank(), crowding_dist(), dom_count(), dom_list(),conditions(), nb_max_t(0){

	this->observations = observations;

	f.resize(PROB_DIM);
	c.resize(CSTR_DIM);
	telescopes_alloc.resize(N_TELESCOPE);
	overlap_index.resize(observations.size());
	genes.reserve(observations.size());

	for(int i = 0; i < (int) observations.size(); i++){

		genes.push_back(* new gene(i));
	}
//
//	gene * g;
//	for(std::vector<Observation>::size_type i = 0; i < observations->size(); i++){
//
//		g = new gene();
//		//g->observation = &observations->at(i);
//		genes.push_back(*g);
//
//		//genes[i] = gene();
//	}

}

chromosome::~chromosome() {
	// TODO Auto-generated destructor stub
}


void chromosome::compute_obj_func() {

	//double tstart = clock();

	//std::cout << "I-----------------------begin objt func " << f.size() <<std::endl;
	f.at(0) = 0.0; //f[0] : number of scheduled observations (maximize this value)
	f.at(1) = 0.0; //f[1] : Average of altitude merits of all observed targets (maximize)
	f.at(2) = 0.0; //f[2] : Average of telescope movement during the whole schedule (minimize)
	f.at(3) = 0.0; //f[3] : number of telescopes used for scheduling (minimize)

	std::vector<double> telescope_dist(N_TELESCOPE, 0.0);
	//telescope_dist.reserve(N_TELESCOPE);
	//telescope_dist.resize(N_TELESCOPE);

	std::vector<Observation> last_teles_obs(N_TELESCOPE);
//	last_teles_obs.reserve(N_TELESCOPE);
	//last_teles_obs.resize(N_TELESCOPE);

	std::vector<int> first(N_TELESCOPE);
//	first.reserve(N_TELESCOPE);
	//first.resize(N_TELESCOPE);

	//std::cout << "first size " << first.size() << std::endl;

	//	std::vector<double> telescope_dist(N_TELESCOPE, 0); //contains distances traveled by each telescope
//	std::vector<Observation> last_teles_obs(N_TELESCOPE, 0); //contains the last observation visited by the telescope
//	std::vector<bool> first(N_TELESCOPE, true); //helping variable for computing the movement of one telescope

	//vars
	ln_equ_posn * pos;
	int i = 0;
	int curr_teles;
	double dist;

	//std::cout << "obj func: genes size " << genes.size() << std::endl;
	//std::cout<< "obj fct : observations size"<< observations.size()<< std::endl;
	//range all the observations vector
	for(i = 0; i < (int) genes.size(); i++){


		//if the observation is scheduled
		if( genes.at(i).is_scheduled(observations.at(i)) ){

			curr_teles = genes.at(i).telescope_used;	//get the telescope id
			telescopes_alloc.at(curr_teles) = 1;	//mark the telescope as used


			f[0]++;	//increase the number of scheduled observations

			//sum altitude merits of all observed targets
			//f[1] += genes.at(i).observation->altituteMerit();
			f.at(1) += observations.at(i).altituteMerit();

			//computing the distance traveled by each telescope

			if(first.at(curr_teles) == 0){ //if it is the first observation, save it and move on

				first.at(curr_teles) = 1;
			}
			else{ //if not, compute the distance with the last observation

				//compute the actual distance traveled
				pos = new ln_equ_posn();
				//pos = genes.at(i).observation->getTarget().getEqCord();
				pos = observations.at(i).getTarget().getEqCord();
				dist = last_teles_obs[curr_teles].getTarget().getAngularDistance(pos);
				telescope_dist[curr_teles] += dist;
			}

			//update the last observation visited of the current telescope
			last_teles_obs.at(curr_teles) = observations.at(i);//* genes.at(i).observation;
			violation_ratio +=  this->is_in_req_time(i) + this->is_above_min_height(*(Schedule::conditions),i);
		}
	}
	violation_ratio /= genes.size();
	//get the average altitude merit
	if(f[0]) f[1] = f[1] / f[0];
	else f[1] = 0.0;

	//get the average distance traveled
	//AND
	//the number of telescopes used
	for(i = 0; i < N_TELESCOPE; i++){

		//Avg dist
		f[2] += telescope_dist[i] / N_TELESCOPE;

		//Num teles
		//std::cout << "tel used "<< i << " = " << telescopes_alloc[i] << std::endl;
		f[3] += telescopes_alloc[i];
	}

	//Converting maximization problems into minimization problems (default)
	f[1] = -f[1];
	f[0] = -f[0];
	//std::cout<< "------------------------------------the end obj func "<<std::endl;
	//printf("Time taken: %.5fs\n", (double)(clock() - tstart)/CLOCKS_PER_SEC);
}

double chromosome::get_obj_func(int index) {

	return this->f.at(index);
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

	//std::cout << c[0] << std::endl;
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
		std::cout << "Obs req id : " << observations.at(i).getRequest()->getReqId() << std::endl;
		std::cout << "Obs id : " << observations.at(i).getObsId()<< std::endl;
		std::cout << "Obs exp time : " << fixed << observations.at(i).getExposureTime() << std::endl;
		std::cout << "Obs min h : " << fixed << observations.at(i).getMinHeight() << std::endl;
		std::cout << "Obs min moon : " << fixed << observations.at(i).getMoonMinSeparation() << std::endl;
		std::cout << "Obs req time : " << fixed << observations.at(i).getReqTime().start << " - "
				<< fixed << observations.at(i).getReqTime().end << std::endl;
		std::cout << "Obs target : " << "dec " << fixed << observations.at(i).getTarget().getEqDec() << " - "
				<< "ra " << fixed << observations.at(i).getTarget().getEqRAsc() << std::endl;
		std::cout << endl;
	}
}

void chromosome::clearDomList() {
	dom_list.clear();
}

void chromosome::incrementDomCount() {
	this->dom_count++;
}

void chromosome::decrementDomCount() {
	this->dom_count--;
}
double chromosome::getDomListOf(int index)
{
	return this->dom_list[index];
}
void chromosome::updateViolationRatio()
{
	int size = this->observations.size();
	violation_ratio = 0.0;
	for(int i = 0; i < size; i++)
	{
		/*int isAboveMinHeight(double JD);
	int isAwayFromMoon(double JD);
	int isInReqTime();*/
		violation_ratio += this->observations[i].isAboveMinHeight(this->genes[i].start_date);
		//std::cout<< "details of violation "<< violation_ratio<< std::endl;
		violation_ratio += this->observations[i].isAwayFromMoon(this->genes[i].start_date);
		//std::cout<< "details of violation "<< violation_ratio<< std::endl;
		violation_ratio +=  this->observations[i].isInReqTime();
		//std::cout<< "observation "<< i << " valeur de violation = "<< violation_ratio<< std::endl;
	}
	this->violation_ratio /= size;
	//std::cout<< "_________________________________"<< std::endl;
}

gene chromosome::getGene(int index)
{
	return genes.at(index);
}



int chromosome::is_above_min_height(Obs_conditions conditions, int index) {

	//get height of object

	if( observations.at(index).isHeightConst() ){

	struct ln_equ_posn eq_coord;
	struct ln_hrz_posn horiz_coord;

	eq_coord.dec = observations.at(index).target.getEqDec();
	eq_coord.ra = observations.at(index).target.getEqRAsc();

	ln_get_hrz_from_equ(&eq_coord, conditions.getObserverPtr(), genes.at(index).start_date, &horiz_coord);

	if( horiz_coord.alt >= observations.at(index).getMinHeight() ) return SUCCESS;
	else return FAILURE;

	}
	else return SUCCESS;
}

int chromosome::is_away_moon(Obs_conditions conditions, int index) {

	if( observations.at(index).isMoonConst()){
		double dist = observations.at(index).getTarget().getMoonAngularDistance(genes.at(index).start_date);

		if( dist > observations.at(index).getMoonMinSeparation() ) return SUCCESS; //good
		else return FAILURE; //bad
	}
	else return SUCCESS;
}

int chromosome::is_in_req_time(int index) {

	if( observations.at(index).isTimeConst() ){

		time_interval requested = observations.at(index).getReqTime();

		if( genes.at(index).start_date >= requested.start
				&& genes.at(index).start_date <= requested.end )

			return SUCCESS; //good
		else return FAILURE; //bad
	}
	else return SUCCESS;
}

int chromosome::is_close_to_meridien(int index) {

	double transit = Schedule::observations.at(index).getTarget().getRiseSetTransit().transit;

	if( transit >= addSecondsToJD(genes.at(index).start_date, -1800) &&
			transit <= addSecondsToJD(genes.at(index).start_date, 1800))
		return 1;
	else return 0;
	return 0;
}

void chromosome::ratio_violated_const(Obs_conditions conditions) {

	double sum = 0.0;
	for(int i = 0;  i < (int) genes.size(); i++){

		sum +=  is_above_min_height(conditions, i) +
<<<<<<< HEAD
				// is_away_moon(conditions, i) +
=======
				 //is_away_moon(conditions, i) +
>>>>>>> 0387e8b643872840f8158d247c8b573c76b05fbf
				 is_in_req_time(i);
	}

	violation_ratio = sum / (double) genes.size();
}

double chromosome::get_duration() {

	double total = 0.0;


		for(int i = 0; (int) i < genes.size(); i++){

			if( genes.at(i).getIsSched() ) total += genes.at(i).duration;
		}

		double dur = total/f.at(3);
		//std::cout << f.at(3)<< " value " << fixed << dur << std::endl;


	return total*1000 / f.at(3);
}

int chromosome::getNbMaxT() {
	return nb_max_t;
}

void chromosome::setNbMaxT(int nbMaxT) {
	nb_max_t = nbMaxT;
}
