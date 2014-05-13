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
#include <iostream>
<<<<<<< HEAD

=======
#include <assert.h>
#include <stack>
>>>>>>> 85f8b6b944e448d6e1d88b92dd3d1efad0edee3b
using namespace std;

class eNFA: public Automaton< std::string,std::set<int> > {
private:
	std::set<int> currentState;

public:
	eNFA(std::vector<State<std::string, std::set<int> > >& states, std::set<std::string>& alphabet);
	virtual ~eNFA();

<<<<<<< HEAD
	set<int> getSubset();
	set<set<int>> getQD();
	set<int> eclose(int indexState, std::set<int>& indexesToIgnore);
	set<int> getStartStateDFA();
	set<int> getAcceptingStatesDFA();

	eNFA regexToeNFA(std::string regex);
=======
	static std::set<int> eclose(const State<std::string,std::set<int>>& theState);
>>>>>>> 85f8b6b944e448d6e1d88b92dd3d1efad0edee3b

	friend std::ostream& operator<< (std::ostream &out, eNFA &enfa);
	friend eNFA operator^(const eNFA &enfa1, const eNFA &enfa2);		//regex.regex
	friend eNFA operator+(const eNFA &enfa1, const eNFA &enfa2);		//regex+regex
	eNFA operator*();							//regex*
};

<<<<<<< HEAD
eNFA geteNFA(std::string alphabet);
=======
eNFA geteNFA(char symbool);
string setPoints(string regex);
eNFA regexToeNFA(std::string regex);
>>>>>>> 85f8b6b944e448d6e1d88b92dd3d1efad0edee3b

#endif /* ENFA_H_ */
