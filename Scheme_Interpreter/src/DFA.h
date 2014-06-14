/*
 * DFA.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#ifndef DFA_H_
#define DFA_H_
using namespace std;
#include "Automaton.h"
#include "generic.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <utility>
#include <functional>

typedef std::function<bool(int,int,std::set<std::set<int>>&)> 
    TestDistinguishabilityType;

class DFA: public Automaton<char,int> {
private:
	int currentState;

public:
	DFA(const std::vector<State<char,int>>& states, const std::set<char>& alphabet);
	DFA();
	virtual ~DFA();

	bool readChar(char theChar);
	bool readString(const std::string& theString);
	bool accepted();
	void reset();
	void eliminateUnreachableStates();
	void minimize();
	void addSymbols(string symbols);

	template<typename ItType1, typename ItType2>
	// ItType should implement postfix increment operator and should 
	//  dereference to a char.
	// Returns an ItType1 pointing to the element that triggered acceptance if
	//  an accepting state was encountered. Otherwise, returns the ItType1
	//  equivalent of end.
	ItType1 readUntilAccepted(ItType1 begin, ItType2 end) {
		auto it = begin;
		for (; it != end; it++)
			if (readChar(*it)) break;
		return it;
	}
	
	friend DFA operator*(const DFA &DFA1, const DFA &DFA2);		//geeft de productautomaat terug van DFA1 en DFA2
	friend std::ostream& operator<< (std::ostream &out, DFA &dfa);
};

#endif /* DFA_H_ */
