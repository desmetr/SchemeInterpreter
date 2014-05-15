/*
 * main.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "DFA.h"
#include "eNFA.h"
#include "readInput.h"
#include <iostream>
#include <map>
#include <utility>

using namespace std;

void checkregexPLUSregex(){
/*	eNFA enfa1 = geteNFA('a');

	eNFA enfa2 = geteNFA('c');


	eNFA enfa3 = enfa1+enfa2;
	eNFA enfa4 = *enfa3;
	eNFA enfa5 = enfa4^enfa1;
	eNFA enfa6 = enfa5^enfa3;*/
	eNFA enfa6 = regexToeNFA("(a+b)abbb*bdf*(uf*+h)");


	ofstream ofStream3;
	ofStream3.open("regexToeNFA.dot");
	ofStream3<<enfa6;
	ofStream3.close();
}

void checkOutputEnfa(){
	set<string> alph = {"a", "b", ""};
	vector<State<string,set<int>> > states;
	states.push_back(State<string,set<int>>());
	set<int> set1 = {1,2};
	states[0].transitions[""] = set1;
	states[0].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set2 = {3};
	states[1].transitions["a"] = set2;
	states[1].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set3 = {4};
	states[2].transitions["b"] = set3;
	states[2].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set4 = {5};
	states[3].transitions[""] = set4;
	states[3].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set5 = {5};
	states[4].transitions[""] = set5;
	states[4].acceptState = false;

	states.push_back(State<string,set<int>>());
	states[5].acceptState = true;

	eNFA eNFA(states, alph);

	ofstream ofStream1;
	ofStream1.open("eNFA1.dot");
	ofStream1<<eNFA;
	ofStream1.close();

}

void getProductautomaat(){
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

	DFA dfa3 = dfa1*dfa2;
	ofstream ofStream3;
	ofStream3.open("productautomaat.dot");
	ofStream3<<dfa3;
	ofStream3.close();

}

void testMinimization() {

    set<char> alphabet = {'0', '1'};
    vector<State<char,int> > states(5);

    // A
    states[0].acceptState = true;
    states[0].transitions['0'] = 0;
    states[0].transitions['1'] = 1;

    // B
    states[1].acceptState = false;
    states[1].transitions['0'] = 0;
    states[1].transitions['1'] = 1;

    // C
    states[2].acceptState = false;
    states[2].transitions['0'] = 3;
    states[2].transitions['1'] = 4;

    // D
    states[3].acceptState = false;
    states[3].transitions['0'] = 3;
    states[3].transitions['1'] = 4;

    // E
    states[4].acceptState = false;
    states[4].transitions['0'] = 2;
    states[4].transitions['1'] = 4;

    DFA test(states, alphabet);
    ofstream eerste;
	eerste.open("eerste.dot");
	eerste << test;

    test.minimize();
    ofstream eersteminimized;
	eersteminimized.open("eersteminimized.dot");
	eersteminimized << test;



    vector<State<char,int> > states2(8);

    // A
    states2[0].acceptState = false;
    states2[0].transitions['0'] = 1;
    states2[0].transitions['1'] = 5;

    // B
    states2[1].acceptState = false;
    states2[1].transitions['0'] = 6;
    states2[1].transitions['1'] = 2;

    // C
    states2[2].acceptState = true;
    states2[2].transitions['0'] = 0;
    states2[2].transitions['1'] = 2;

    // D
    states2[3].acceptState = false;
    states2[3].transitions['0'] = 2;
    states2[3].transitions['1'] = 6;

    // E
    states2[4].acceptState = false;
    states2[4].transitions['0'] = 7;
    states2[4].transitions['1'] = 5;

    // F
    states2[5].acceptState = false;
    states2[5].transitions['0'] = 2;
    states2[5].transitions['1'] = 6;
    
    // G
    states2[6].acceptState = false;
    states2[6].transitions['0'] = 6;
    states2[6].transitions['1'] = 4;

    // H
    states2[7].acceptState = false;
    states2[7].transitions['0'] = 6;
    states2[7].transitions['1'] = 2;

	ofstream output;
	ofstream output2;
	output.open("unminimized.dot");
    DFA testMin(states2, alphabet);
    output << testMin;
	output2.open("minimized.dot");
    testMin.minimize();
    output2 << testMin;
    

    // fuck yea het werkt
}

void convertENFAtoDFA()	{
	set<string> alph = {"a", "b", ""};
	vector<State<string,set<int>> > states;
	states.push_back(State<string,set<int>>());
	set<int> set1 = {1,2};
	states[0].transitions[""] = set1;
	states[0].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set2 = {3};
	states[1].transitions["a"] = set2;
	states[1].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set3 = {4};
	states[2].transitions["b"] = set3;
	states[2].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set4 = {5};
	states[3].transitions[""] = set4;
	states[3].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set5 = {5};
	states[4].transitions[""] = set5;
	states[4].acceptState = false;

	states.push_back(State<string,set<int>>());
	states[5].acceptState = true;

	eNFA enfa = eNFA(states, alph);

	//cout << states[0].transitions["0"][0] << endl;

}

int main(int argc, char const* argv[]) {
/*	checkregexPLUSregex();
	
	set<string> alph = {"a", "b", ""};
	vector<State<string,set<int>> > states;
	states.push_back(State<string,set<int>>());
	set<int> set1 = {1,2};
	states[0].transitions[""] = set1;
	states[0].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set2 = {3};
	states[1].transitions["a"] = set2;
	states[1].transitions[""] = {5};
	states[1].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set3 = {4};
	states[2].transitions["b"] = set3;
	states[2].transitions[""] = set<int>();
	states[2].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set4 = {5};
	states[3].transitions[""] = set4;
	states[3].acceptState = false;

	states.push_back(State<string,set<int>>());
	set<int> set5 = {5};
	states[4].transitions[""] = set5;
	states[4].acceptState = false;

	states.push_back(State<string,set<int>>());
	states[5].transitions[""] = set<int>();
	states[5].acceptState = true;

	eNFA eNFA(states, alph);

    set<int> empty;
	set<int> mySet = eNFA.eclose(0, empty);
	// Doen allebei hetzelfde.
	set<int> mySet2 = eNFA.getStartStateDFA();

	// Itereren over de set verkregen door de eclose(0).
	for (set<int>::iterator it = mySet.begin(); it != mySet.end(); it++)	{
		cout << *it << endl;
	}
	// Itereren over de set verkregen door getStartStateDFA().
	for (set<int>::iterator it = mySet2.begin(); it != mySet2.end(); it++)	{
		cout << *it << endl;
	}

	testMinimization();*/
	input(argc,argv);

	cout<<"end"<<endl;
	return 0;

}
