/*
 * targettest.h
 *
 *  Created on: 15 août 2016
 *      Author: root
 */

#ifndef TARGETTEST_H_
#define TARGETTEST_H_

class target_test {
public:
	target_test(double x, double y);
	virtual ~target_test();

	double getX() const;
	void setX(double x);
	double getY() const;
	void setY(double y);

	double add();

private:
	double x;
	double y;
};

#endif /* TARGETTEST_H_ */
