/*
 * main.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "DFA.h"
#include <iostream>
#include <map>
#include <utility>

using namespace std;

int main() {
	set<char> alph = {'a', 'b'};
	vector<State> states;
	states.push_back(State());
	states[0].transitions['a'] = set<int>({0});
	states[0].transitions['b'] = set<int>({1});
	states[0].acceptState = false;

	states.push_back(State());
	states[1].transitions['a'] = set<int>({1});
	states[1].transitions['b'] = set<int>({0});
	states[1].acceptState = true;

	DFA dfa = DFA(states, alph);

	cout << dfa.readString("a") << endl;
	dfa.reset();
	cout << dfa.readString("ab") << endl << endl;
	dfa.reset();
	cout << dfa.readString("abbaaaab") << endl;
}
