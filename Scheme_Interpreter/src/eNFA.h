/*
 * eNFA.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#ifndef ENFA_H_
#define ENFA_H_

#include <set>
#include <string>
#include "Automaton.h"
#include <iostream>
using namespace std;

class eNFA: public Automaton< std::string,std::set<int> > {
private:
	std::set<int> currentState;

public:
	eNFA(vector<State<string,set<int> > >& states, set<string>& alphabet);
	virtual ~eNFA();

	static std::set<int> eclose(const State<std::string,std::set<int>>& theState);
	eNFA regexToeNFA(std::string regex);

	friend std::ostream& operator<< (std::ostream &out, eNFA &enfa);
	friend eNFA operator^(const eNFA &enfa1, const eNFA &enfa2);		//regex.regex
	friend eNFA operator+(const eNFA &enfa1, const eNFA &enfa2);		//regex+regex
	eNFA operator*();													//regex*
};

eNFA geteNFA(string alphabet);

#endif /* ENFA_H_ */
