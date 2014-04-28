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

class eNFA: public Automaton< std::string,std::set<int> > {
private:
	std::set<int> currentState;

public:
	eNFA(const std::vector<State<std::string,std::set<int> > >& states, const std::set<char>& alphabet);
	virtual ~eNFA();

	static std::set<int> eclose(const State<std::string,std::set<int>>& theState);
};

#endif /* ENFA_H_ */
