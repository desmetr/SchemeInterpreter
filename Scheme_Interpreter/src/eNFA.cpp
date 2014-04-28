/*
 * eNFA.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "eNFA.h"


eNFA::eNFA(const std::vector<State<std::string,std::set<int> > >& states, const std::set<char>& alphabet)
: Automaton(states, alphabet)
{

}

eNFA::~eNFA() {
	// TODO Auto-generated destructor stub
}

std::set<int> eNFA::eclose(const State<std::string,std::set<int>>& theState) {

}

