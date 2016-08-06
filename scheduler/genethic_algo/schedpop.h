/*
 * schedpop.h
 *
 *  Created on: 24 juin 2016
 *      Author: root
 */

#ifndef SCHEDPOP_H_
#define SCHEDPOP_H_

#include <population.h>
#include <config.h>
#include <serialization.h>
#include <types.h>

#include "SchedulingProb.h"

namespace pagmo {

class sched_pop : public population {
public:
	sched_pop(const problem::base &p, int n, const boost::uint32_t &seed);
	virtual ~sched_pop();
private:
	void reinit(const size_type &);

};

} /* namespace algorithm */

#endif /* SCHEDPOP_H_ */
