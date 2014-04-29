/*
 * Automaton.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#ifndef AUTOMATON_H_
#define AUTOMATON_H_
#include <map>
#include <set>
#include <vector>
#include <string>
#include <fstream>

template<typename keyType, typename valueType>
struct State {
	std::map<keyType, valueType> transitions;
	bool acceptState;
};

/*struct State {
	std::map<char, std::set<int> > transitions;
	bool acceptState;
};*/

template<typename keyType, typename valueType>
class Automaton {
protected:
// Attributes used in subclasses.
	std::vector<State<keyType,valueType> > states;
	std::set<keyType> alphabet;

public:
	Automaton(std::vector<State<keyType,valueType> > states, std::set<keyType> alphabet)
	: states(states)
	, alphabet(alphabet)
	{}
	virtual ~Automaton() {}
};

#endif /* AUTOMATON_H_ */
