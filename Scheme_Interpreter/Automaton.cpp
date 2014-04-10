/*
 * Automaton.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "Automaton.h"

Automaton::Automaton(std::vector<State> states, std::set<char> alphabet)
: states(states)
, alphabet(alphabet)
{
	// do nothing
}

Automaton::~Automaton() {
	// TODO Auto-generated destructor stub
}

