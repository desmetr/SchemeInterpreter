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
#include <map>
#include "Automaton.h"
#include <iostream>
#include <algorithm>

#include <assert.h>
#include <stack>
#include <utility>
#include "generic.h"

using namespace std;

class DFA;

class eNFA: public Automaton< std::string,std::set<int> > {
public:
	eNFA(std::vector<State<std::string, std::set<int> > >& states, std::set<std::string>& alphabet);
	virtual ~eNFA();

	set<set<int>> getQD();
	set<int> eclose(int indexState, std::set<int>& indexesToIgnore);
	set<int> eclose(set<int> subset);
	DFA modSubCnstr();

	eNFA regexToeNFA(std::string regex);

	friend std::ostream& operator<< (std::ostream &out, eNFA &enfa);
	friend eNFA operator^(const eNFA &enfa1, const eNFA &enfa2);		//regex.regex
	friend eNFA operator+(const eNFA &enfa1, const eNFA &enfa2);		//regex+regex
	eNFA operator*();							//regex*
	
private:
	std::set<int> currentState;
	
	// This map records for each reachable subset of eNFA states SBS and for each symbol SYM, 
	// the subset of eNFA states reached after transitioning from SBS over SYM
	typedef std::map<std::set<int>, std::map<string, std::set<int>> > TransitionType;
	TransitionType transitions;
	
	std::set<int> eNFA_transition(std::set<int>& subset, const string& symbol);
	void constructTransitions(std::set<int>& subset);
	void constructDFA_States(std::set<int>& subset, std::map<std::set<int>, int>& stateToIndexMap, std::vector<State<char,int>>& DFA_States);
};

eNFA geteNFA(string alphabet);
eNFA geteNFA(char symbool);
string setPoints(string regex);
eNFA regexToeNFA(string regexBegin);
void replace(string& input);

#include "DFA.h"

#endif /* ENFA_H_ */
