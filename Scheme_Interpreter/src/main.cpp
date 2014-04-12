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

void getProductautomaat(){
	cout<<"lala1"<<endl;
	set<char> alph = {'0', '1'};
	vector<State<char,int> > states;
	states.push_back(State<char,int>());
	states[0].transitions['0'] = 0;
	states[0].transitions['1'] = 1;
	states[0].acceptState = false;

	states.push_back(State<char,int>());
	states[1].transitions['1'] = 0;
	states[1].transitions['0'] = 1;
	states[1].acceptState = true;

	DFA dfa1 = DFA(states, alph);

	ofstream ofStream1;
	ofStream1.open("dfa1.dot");
	ofStream1<<dfa1;
	ofStream1.close();


	set<char> alph2 = {'0','1'};
	vector<State<char,int> > states2;
	states2.push_back(State<char,int>());
	states2[0].transitions['1'] = 0;
	states2[0].transitions['0'] = 1;
	states2[0].acceptState = false;

	states2.push_back(State<char,int>());
	states2[1].transitions['0'] = 2;
	states2[1].transitions['1'] = 0;
	states2[1].acceptState = false;

	states2.push_back(State<char,int>());
	states2[2].transitions['0'] = 3;
	states2[2].transitions['1'] = 0;
	states2[2].acceptState = false;

	states2.push_back(State<char,int>());
	states2[3].transitions['0'] = 3;
	states2[3].transitions['1'] = 3;
	states2[3].acceptState = true;

	DFA dfa2 = DFA(states2, alph2);
	ofstream ofStream2;
	ofStream2.open("dfa2.dot");
	ofStream2<<dfa2;
	ofStream2.close();

	cout<<"lala2"<<endl;
	DFA dfa3 = dfa1*dfa2;
	cout<<"lala3"<<endl;
	ofstream ofStream3;
	ofStream3.open("productautomaat.dot");
	ofStream3<<dfa3;
	ofStream3.close();

}

int main() {
	getProductautomaat();
	cout<<"end"<<endl;
	return 0;
}
