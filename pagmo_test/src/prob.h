/*
 * prob.h
 *
 *  Created on: 17 juin 2016
 *      Author: root
 */

#ifndef PROB_H_
#define PROB_H_

#include <string>

#include <config.h>
#include <serialization.h>
#include <types.h>
#include <problem/base.h>

namespace pagmo { namespace problem {

class __PAGMO_VISIBLE prob : public base {
public:
	prob(unsigned int dimension);
	virtual ~prob();

	base_ptr clone() const;
	void objfun_impl(fitness_vector &, const decision_vector &) const;

private:
	friend class boost::serialization::access;
	template <class Archive>
	void serialize(Archive &ar, const unsigned int)
	{
	    ar & boost::serialization::base_object<base>(*this);
	}
};
}
}


BOOST_CLASS_EXPORT_KEY(pagmo::problem::prob)
#endif /* PROB_H_ */
