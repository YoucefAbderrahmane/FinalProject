#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Observer{

private:
	int id;
	string email;
	double quota;
	int priority;
	vector <int> vect;

public:
	const string& getEmail() const {
		return email;
	}

	void setEmail(const string& email) {
		this->email = email;
	}

	int getId() const {
		return id;
	}

	void setId(int id) {
		this->id = id;
	}

	int getPriority() const {
		return priority;
	}

	void setPriority(int priority) {
		this->priority = priority;
	}

	double getQuota() const {
		return quota;
	}

	void setQuota(double quota) {
		this->quota = quota;
	}

	Observer(int id, string email, double quota, int priority){

		this->id = id;
		this->email = email;
		this->quota = quota;
		this->priority = priority;
	}

	Observer();

	~Observer();
};
