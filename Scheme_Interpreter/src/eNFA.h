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
using namespace std;

class eNFA: public Automaton< std::string,std::set<int> > {
private:
	std::set<int> currentState;

public:
	eNFA(vector<State<string,set<int> > >& states, set<string>& alphabet);
	virtual ~eNFA();

	static std::set<int> eclose(const State<std::string,std::set<int>>& theState);
	eNFA regexToeNFA(std::string regex);
	eNFA regexPLUSregex(eNFA enfa1, eNFA enfa2);
	eNFA regexMAALregex(eNFA enfa1, eNFA enfa2);
	eNFA regexSTER(eNFA enfa);

	friend std::ostream& operator<< (std::ostream &out, eNFA &enfa);
};

#endif /* ENFA_H_ */
