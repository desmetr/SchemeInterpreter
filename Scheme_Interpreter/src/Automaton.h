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

template<typename KeyType, typename ValueType>
struct State {
	std::map<KeyType, ValueType> transitions;
	bool acceptState;
	State(){
		acceptState=false;
	}
};

/*struct State {
	std::map<char, std::set<int> > transitions;
	bool acceptState;
};*/

template<typename KeyType, typename ValueType>
class Automaton {
protected:
// Attributes used in subclasses.
	std::vector<State<KeyType,ValueType> > states;
	std::set<KeyType> alphabet;

public:
	Automaton(std::vector<State<KeyType,ValueType> > states, std::set<KeyType> alphabet)
	: states(states)
	, alphabet(alphabet)
	{}

	void setAlphabet(std::set<KeyType> newAlphabet) {alphabet = newAlphabet;}
	virtual ~Automaton() {}
};

#endif /* AUTOMATON_H_ */
