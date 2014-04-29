/*
 * DFA.h
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#ifndef DFA_H_
#define DFA_H_
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
    void minimize();

public:
	DFA(const std::vector<State<char,int>>& states, const std::set<char>& alphabet);
	virtual ~DFA();

	bool readChar(char theChar);
	bool readString(const std::string& theString);
	bool readUntilAccepted(const std::string& theString);
	bool accepted();
	void reset();
	friend DFA operator*(const DFA &DFA1, const DFA &DFA2);		//geeft de productautomaat terug van DFA1 en DFA2
	friend std::ostream& operator<< (std::ostream &out, DFA &cPoint);
};

#endif /* DFA_H_ */
