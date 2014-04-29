/*
 * eNFA.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "eNFA.h"


eNFA::eNFA(std::vector<State<std::string,std::set<int> > >& states, std::set<string>& alphabet)
: Automaton(states, alphabet)
{

}

eNFA::~eNFA() {
	// TODO Auto-generated destructor stub
}

std::set<int> eNFA::eclose(const State<std::string,std::set<int>>& theState) {

}

eNFA eNFA::regexToeNFA(std::string regex) {

}

eNFA eNFA::regexPLUSregex(eNFA enfa1, eNFA enfa2) {

	std::vector<State<string,set<int>>> states;
	int beginIndex = enfa1.states.size()+1;

	State<string,set<int>> headState;
	std::set<int> reach1;
	reach1.insert(1);
	reach1.insert(beginIndex);
	headState.transitions.at("")=reach1;
	states.push_back(headState);

	for(vector<State<string,set<int>>>::iterator state = enfa1.states.begin() ; state!=enfa1.states.end() ; state++){
		for(map<string,set<int>>::iterator transition = state->transitions.begin() ; transition!=state->transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+1);
			}
			transition->second=newSet;
		}
		states.push_back(*state);
	}


	for(vector<State<string,set<int>>>::iterator state = enfa1.states.begin() ; state!=enfa1.states.end() ; state++){
		for(map<string,set<int>>::iterator transition = state->transitions.begin() ; transition!=state->transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+beginIndex);
			}
			transition->second=newSet;
		}
		states.push_back(*state);
	}

	State<string,set<int>> tailState;
	tailState.acceptState=true;
	states.push_back(tailState);
	states.at(beginIndex).transitions.at("").insert(beginIndex-1);
	states.at(beginIndex).transitions.at("").insert(states.size());
}
std::ostream& operator<< (std::ostream &out, eNFA &enfa){

	out<<"digraph eNFA {"<<endl;
	out<<"\trankdir=LR;"<<endl<<endl;
	int i=0;
	for(std::vector<State<string,set<int> >>::iterator state = enfa.states.begin() ; state!=enfa.states.end() ; state++){
		if(state->acceptState){
			out<<"\t"<<i<<" [shape=doublecircle];"<<endl;
		}
		else{
			out<<"\t"<<i<<" [shape=circle];"<<endl;
		}
		for(std::set<string>::iterator caracter=enfa.alphabet.begin() ; caracter!=enfa.alphabet.end() ; caracter++){
			if(state->transitions.find(*caracter)!=state->transitions.end()){
				for(set<int>::iterator index=state->transitions.at(*caracter).begin() ; index != state->transitions.at(*caracter).end() ; index++  ){
					if((*caracter)==""){
						out<<"\t"<<i<<" -> "<<(*index)<<" [label=\""<<"ε"<<"\"];"<<endl;

					}
					else{
						out<<"\t"<<i<<" -> "<<(*index)<<" [label=\""<<*caracter<<"\"];"<<endl;
					}

				}
			}
		}
		out<<endl;
		i++;
	}
	out<<"}"<<std::endl;
	return out;
}



eNFA eNFA::regexMAALregex(eNFA enfa1, eNFA enfa2) {
}

eNFA eNFA::regexSTER(eNFA enfa) {
}
