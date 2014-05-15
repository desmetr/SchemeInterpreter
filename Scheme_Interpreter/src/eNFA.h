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
#include "DFA.h"
#include <iostream>

#include <assert.h>
#include <stack>
#include <utility>
#include "generic.h"

using namespace std;

class eNFA: public Automaton< std::string,std::set<int> > {
private:
	std::set<int> currentState;

public:
	eNFA(std::vector<State<std::string, std::set<int> > >& states, std::set<std::string>& alphabet);
	virtual ~eNFA();

	//set<int> getSubset();
	set<set<int>> getQD();
	set<int> eclose(int indexState, std::set<int>& indexesToIgnore);
	set<int> eclose(set<int> subset) const;
	DFA modSubCnstr() const;
	set<int> getStartStateDFA();
	set<set<int>> getAcceptingStatesDFA();

	friend std::ostream& operator<< (std::ostream &out, eNFA &enfa);
	friend eNFA operator^(const eNFA &enfa1, const eNFA &enfa2);		//regex.regex
	friend eNFA operator+(const eNFA &enfa1, const eNFA &enfa2);		//regex+regex
	eNFA operator*();							//regex*
};

eNFA geteNFA(std::string alphabet);
eNFA geteNFA(char symbool);
string setPoints(string regex);
eNFA regexToeNFA(std::string regex);

#endif /* ENFA_H_ */
