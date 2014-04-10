/*
 * DFA.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "DFA.h"

DFA::DFA(const std::vector<State>& states, const std::set<char>& alphabet)
: Automaton(states, alphabet)
{
	currentState = 0;
}

DFA::~DFA() {
	// TODO Auto-generated destructor stub
}

bool DFA::readChar(char theChar) {
	currentState = *(states.at(currentState).transitions.at(theChar).begin());
	return accepted();
}

bool DFA::readString(const std::string& theString) {
	for (char c : theString) readChar(c);
	return accepted();
}

bool DFA::readUntilAccepted(const std::string& theString) {
	for (char c : theString) if (readChar(c)) return true;
	return false;
}

bool DFA::accepted() {
	return states.at(currentState).acceptState;
}

void DFA::reset() {
	currentState = 0;
}
