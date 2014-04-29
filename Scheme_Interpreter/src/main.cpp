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
    states[2].acceptState = true;
    states[2].transitions['0'] = 3;
    states[2].transitions['1'] = 4;

    // D
    states[3].acceptState = true;
    states[3].transitions['0'] = 3;
    states[3].transitions['1'] = 4;

    // E
    states[4].acceptState = false;
    states[4].transitions['0'] = 2;
    states[4].transitions['1'] = 4;

    DFA test(states, alphabet);
    

    cout << "____________" << endl << endl;

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

    DFA(states2, alphabet);
    // fuck yea het werkt
}

int main() {
	//getProductautomaat();
	//cout<<"end"<<endl;
	//return 0;
    testMinimization();
}
