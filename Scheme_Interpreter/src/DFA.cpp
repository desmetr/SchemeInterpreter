/*
 * DFA.cpp
 *
 *  Created on: Apr 10, 2014
 *	  Author: ajay
 */

#include "DFA.h"
//static int callctr = 0;
DFA::DFA() : Automaton(states, alphabet){
}
DFA::DFA(const std::vector<State<char,int> >& states, const std::set<char>& alphabet)
: Automaton(states, alphabet)
{
	currentState = 0;
}

DFA::~DFA() {
	// TODO Auto-generated destructor stub
}


bool DFA::readChar(char theChar) {
	if(this->alphabet.find(theChar) != this->alphabet.end()){
		currentState = states.at(currentState).transitions.at(theChar);
	}
	else{
		cout<<"\"" << theChar <<"\" maakt geen deel uit het alphabet"<<endl;
	}
	return accepted();
}

bool DFA::readString(const std::string& theString) {
	for (char c : theString) readChar(c);
	return accepted();
}

bool DFA::accepted() {
	return states.at(currentState).acceptState;
}

void DFA::reset() {
	currentState = 0;
}

void DFA::eliminateUnreachableStates() {
	// create a list of unreached states, add all states
	std::set<int> unreachedStates;
	for (int i = 0; i < states.size(); i++)
		unreachedStates.insert(i);

	// remove states that can be reached from the starting state
	std::function<void(int)> findReachableStates = [&](int index) -> void {
		// if the current state has not been reached yet, investigate it
		// and all states that can be reached directly from it
		if (unreachedStates.erase(index))
			for (char c: alphabet)
				findReachableStates(states.at(index).transitions.at(c));
		// if the state has already been reached, just ignore it
		else return;
	};
	findReachableStates(0);
	// now, we eliminate the states that couldn't be reached
	//
	// our first task is to identify the new indices the reachable states
	//  will get
	std::map<int,int> oldIndexToNewIndex;
	int offset = 0;
	for (int i = 0; i < states.size(); i++) {
		if (unreachedStates.count(i))
			offset++;
		else
			oldIndexToNewIndex.insert(std::make_pair(i, i-offset));
	}
	std::vector<State<char,int>> newStates;
	for (int i = 0; i < states.size(); i++) {
		if (! unreachedStates.count(i)) {
			newStates.push_back(states[i]);
			for (char c: alphabet) {
				newStates.back().transitions.at(c) =
						oldIndexToNewIndex.at(newStates.back().transitions.at(c));
			}
		}
	}
	states = newStates;
}

void DFA::minimize() {
	// tests whether two indices refer to distinguishable states
	// pairsToIgnore contains pairs that have been encountered but whose
	// distinguishability has not yet been determined (we ignore them to avoid
	// entering an infinite recursion)
	TestDistinguishabilityType testDistinguishability = memoize(
			TestDistinguishabilityType([&](int i, int j,
					std::set< std::set<int> >& pairsToIgnore)
					{
		if (i == j) {
			//a state is equivalent with itself
			pairsToIgnore.erase({i,j});
			return false;
		} else if (states[i].acceptState != states[j].acceptState) {
			//an accepting state is always distinguishable from a
			// non-accepting state
			pairsToIgnore.erase({i,j});
			return true;
		} else {
			//if neither of the base cases apply, recursively search for a
			// distinguishable pair that can be reached from the current states
			// with an identical string; if no such pair can be found, the
			// result is false, otherwise it is true
			pairsToIgnore.insert({i,j});
			bool result = any(alphabet, [&](char c) {
				int nextI = states[i].transitions.at(c);
				int nextJ = states[j].transitions.at(c);
				//if the next pair to examine has already been encountered
				//in this specific search, ignore this pair by returning
				//false without memoizing the result
				if (pairsToIgnore.find({nextI,nextJ})
						!= pairsToIgnore.end()) return false;
				return testDistinguishability(nextI, nextJ, pairsToIgnore);
			});
			pairsToIgnore.erase({i,j});
			return result;
		}
					}));
	// first, we eliminate unreachable states
	eliminateUnreachableStates();

	// then, we sort all equivalent states in equivalence classes
	std::vector<std::set<int>> equivalenceClasses;
	std::map<int,int> stateToEquivalenceClass;
	for (int i = 0; i < states.size(); i++) {
		if (stateToEquivalenceClass.find(i) != stateToEquivalenceClass.end())
			continue;
		//if the state we are currently examining has not yet been placed in an
		// equivalence class, create a new equivalence class and find its members
		// (otherwise, skip to the next state)
		equivalenceClasses.push_back(std::set<int>({i}));
		int ecIndex = equivalenceClasses.size() - 1;
		stateToEquivalenceClass.insert(std::make_pair(i, ecIndex));
		for (int j = i+1; j < states.size(); j++) {
			std::set< std::set<int> > pairsToIgnore;
			//if we find an equivalent state, place it in i's equivalence class
			if (!testDistinguishability(i, j, pairsToIgnore)) {
				equivalenceClasses.back().insert(j);
				stateToEquivalenceClass.insert(std::make_pair(j, ecIndex));
			}
		}
	}
	//now we construct the new states from the equivalence classes
	std::vector<State<char,int>> minimizedStates;
	for (int ecIndex = 0; ecIndex < equivalenceClasses.size(); ecIndex++) {
		minimizedStates.push_back(State<char,int>());
		minimizedStates.back().acceptState =
				states[*(equivalenceClasses[ecIndex].begin())].acceptState;
		//initialize the minimized state as one referring just to itself
		//overwrite the transition for a given character if one of the states
		// in the current equivalence class refers to a state in another
		// equivalence class
		for (char c: alphabet) {
			minimizedStates.back().transitions[c] = ecIndex;
			for (int stateIndex: equivalenceClasses[ecIndex]) {
				int oldStateTransIndex = states[stateIndex].transitions[c];
				int ecTransIndex = stateToEquivalenceClass[oldStateTransIndex];
				if (ecTransIndex != ecIndex) {
					minimizedStates.back().transitions[c] = ecTransIndex;
					break;
				}
			}
		}
	}
	states = minimizedStates;
}

DFA operator *(const DFA& DFA1, const DFA& DFA2) {
	std::map<std::pair<int,int>,int> newStates;
	int z = 0;
	for(unsigned int i=0 ; i<DFA1.states.size() ; i++){
		for(unsigned int j=0 ; j<DFA2.states.size() ; j++){
			newStates[std::make_pair(i,j)]=z;
			z++;
		}
	}

	std::vector<State<char,int>> states;
	std::set<char> newAlph = DFA1.alphabet;
	newAlph.insert(DFA2.alphabet.begin(),DFA2.alphabet.end());

	for(unsigned int i=0 ; i<DFA1.states.size() ; i++){
		for(unsigned int j=0 ; j<DFA2.states.size() ; j++){
			std::pair<int,int> stateNumber;
			State<char,int> state;

			if(DFA1.states.at(i).acceptState || DFA2.states.at(j).acceptState)	state.acceptState=true;
			else																state.acceptState=false;

			for(std::set<char>::iterator caracter=newAlph.begin() ; caracter!=newAlph.end() ; caracter++){
				if(DFA1.alphabet.find(*caracter)==DFA1.alphabet.end())  	stateNumber.first=i;
				else														stateNumber.first=DFA1.states.at(i).transitions.at(*caracter);

				if(DFA2.alphabet.find(*caracter)==DFA2.alphabet.end())  	stateNumber.second=j;
				else														stateNumber.second=DFA2.states.at(j).transitions.at(*caracter);

				state.transitions[*caracter] = newStates.at(stateNumber);
			}
			states.push_back(state);
		}
	}
	return DFA(states,newAlph);
}

void DFA::addSymbols(string symbols) {
	for(char sym : symbols){
		if( this->alphabet.find(sym) == this->alphabet.end()){
			cout<<sym<<endl;
			this->alphabet.insert(sym);
			for(auto &state : this->states){
				state.transitions[sym] = this->states.size();
			}
		}
	}
	State<char,int> state;
	for(char sym : this->alphabet){
		state.transitions[sym] = this->states.size();
	}
	this->states.push_back(state);
}

std::ostream& operator<< (std::ostream &out, DFA &dfa){
	out<<"digraph DFA {"<<std::endl;
	out<<"\trankdir=LR;";
	int i=0;
	for(std::vector<State<char,int>>::iterator state = dfa.states.begin() ; state!=dfa.states.end() ; state++){
		if(state->acceptState){
			out<<"\t"<<i<<" [shape=doublecircle];"<<std::endl;
		}
		else{
			out<<"\t"<<i<<" [shape=circle];"<<std::endl;
		}
		for(std::set<char>::iterator caracter=dfa.alphabet.begin() ; caracter!=dfa.alphabet.end() ; caracter++){
			if(state->transitions.find(*caracter)!=state->transitions.end()){
				out<<"\t"<<i<<" -> "<<state->transitions.at(*caracter)<<" [label=\""<<*caracter<<"\"];"<<std::endl;
			}
		}
		i++;
	}
	out<<"}"<<std::endl;
	return out;
}
