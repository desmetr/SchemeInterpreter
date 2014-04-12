/*
 * DFA.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "DFA.h"

DFA::DFA(const std::vector<State<char,int> >& states, const std::set<char>& alphabet)
: Automaton(states, alphabet)
{
	currentState = 0;
}

DFA::~DFA() {
	// TODO Auto-generated destructor stub
}

bool DFA::readChar(char theChar) {
	currentState = states.at(currentState).transitions.at(theChar);
	return accepted();
}

bool DFA::readString(const std::string& theString) {
	for (char c : theString) readChar(c);
	return accepted();
}

bool DFA::readUntilAccepted(const std::string& theString) {
	for (char c : theString) if (readChar(c)) return true;
	return false;
}

bool DFA::accepted() {
	return states.at(currentState).acceptState;
}

void DFA::reset() {
	currentState = 0;
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




