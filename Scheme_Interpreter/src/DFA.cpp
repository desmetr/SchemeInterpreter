/*
 * DFA.cpp
 *
 *  Created on: Apr 10, 2014
 *      Author: ajay
 */

#include "DFA.h"
static int callctr = 0;
DFA::DFA(const std::vector<State<char,int> >& states, const std::set<char>& alphabet)
: Automaton(states, alphabet)
{
	currentState = 0;
}

DFA::~DFA() {
	// TODO Auto-generated destructor stub
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
    std::cout << minimizedStates.size() << std::endl;
    for (auto& s: minimizedStates) {
        for (auto& p: s.transitions)
            std::cout << p.first << ", " << p.second << std::endl;
        std::cout << std::endl;
    }
    states = minimizedStates;
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
