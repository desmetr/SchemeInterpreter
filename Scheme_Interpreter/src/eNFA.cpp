/*
 * eNFA.cpp
 *
 *  Created on: Apr 10, 2014
 *	  Author: ajay
 */

#include "eNFA.h"


eNFA::eNFA(vector<State<string, set<int> > >& states, set<string>& alphabet)
: Automaton(states, alphabet)	{
}

eNFA::~eNFA() {
}

// Returns the eclosed states of a given state of the eNFA.
set<int> eNFA::eclose(int indexState, set<int>& indexesToIgnore) const {
	if (indexesToIgnore.count(indexState))	return set<int>();

	set<int> localSet;
	localSet.insert(indexState);
	indexesToIgnore.insert(indexState);
	if(states[indexState].transitions.count("") == 1) {
		for (int i : states[indexState].transitions.at(""))	{
			for (int j : eclose(i, indexesToIgnore))	{
				localSet.insert(j);
			}
		}
		return localSet;	
	}
	else	{
		set<int> dumpSet;
		dumpSet.insert(indexState);
		return dumpSet;
	}
}

// Returns all the eclosed sets of a given subset of the eNFA.
set<int> eNFA::eclose(set<int> subset) const {
	set<int> result;
	for (int elem: subset) {
		set<int> toIgnore;
		set<int> elemEclose = eclose(elem, toIgnore);
		for (const auto& elemEcloseElem: elemEclose)
			result.insert(elemEcloseElem);
	}
	return result;
}

DFA eNFA::modSubCnstr() const {
	auto eNFA_transition = [&](const set<int>& subset, const string& symbol) 
		-> set<int>
	{
		set<int> result;
		for (int stateIndex : subset) {
			if (states[stateIndex].transitions.count(symbol) == 0) continue;
			set<int> target = states[stateIndex].transitions.at(symbol);
			result.insert(target.begin(), target.end());
		}
		result = eclose(result);
		return result;
	};

	typedef map< set<int> , map<string,set<int>> > TransitionType;

	// This map records for each reachable subset of eNFA states SBS and for each symbol SYM, 
	//  the subset of eNFA states reached after transitioning from SBS over SYM
	TransitionType transitions;

	// This function recursively fills the 'transitions' map
	function<void(const set<int>&)> constructTransitions = [&](const set<int>& subset)
	{
		transitions.insert(make_pair(subset, map<string,set<int>>()));
		for (const string& symbol : alphabet) {
			set<int> target = eclose(eNFA_transition(subset, symbol));
			transitions.at(subset).insert(make_pair(symbol, target));
			if (transitions.count(target) == 0)
				constructTransitions(target);
		}
	};
	constructTransitions(eclose({0}));

	map<set<int>,int> stateToIndexMap;
	vector<State<char,int>> DFA_States;
	DFA_States.reserve(transitions.size());
	function<void(const set<int>&)> constructDFA_States = [&](const set<int>& subset)
	{
		if (stateToIndexMap.count(subset) == 1) return;
		DFA_States.push_back(State<char,int>());
		unsigned int myIndex = DFA_States.size() - 1;
		stateToIndexMap.insert(make_pair(subset, myIndex));
		for (int stateIndex : subset) {
			if (states[stateIndex].acceptState) {
				DFA_States[myIndex].acceptState = true;
				break;
			}
		}
		
		for (const string& symbol : alphabet) {
			if (symbol == "") continue;
			set<int> target = transitions.at(subset).at(symbol);
			if (stateToIndexMap.count(target) == 0)
				constructDFA_States(target);
			DFA_States[myIndex].transitions.insert(make_pair(symbol[0], stateToIndexMap.at(target)));
		}
	};

	constructDFA_States(eclose({0}));

	set<char> DFA_Alphabet;
	for (const string& symbol : alphabet)
		if (symbol != "") DFA_Alphabet.insert(symbol[0]);

	return DFA(DFA_States, DFA_Alphabet);
}

eNFA eNFA::operator *() {
	std::vector<State<string,set<int>>> states;

	State<string,set<int>> headState;
	std::set<int> reach1 = {1,(int(this->states.size())+1)};

	headState.transitions[""]=reach1;
	states.push_back(headState);

	for(vector<State<string,set<int>>>::iterator state = this->states.begin() ; state!=this->states.end() ; state++){
		State<string,set<int>> newState;
		for(map<string,set<int>>::iterator transition = state->transitions.begin() ; transition!=state->transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+1);
			}
			newState.transitions[transition->first]=newSet;
		}
		states.push_back(newState);
	}

	State<string,set<int>> tailState;
	tailState.acceptState=true;
	states.push_back(tailState);

	states.at(states.size()-2).acceptState=false;

	set<int> set2= {int(states.size()-1),1};
	states.at(states.size()-2).transitions[""]=set2;
	eNFA neweNFA(states,this->alphabet);
	return neweNFA;

}

eNFA operator ^(const eNFA &enfa1, const eNFA &enfa2) {
	vector<State<string,set<int>>> newStates = enfa1.states;
	newStates.insert(newStates.end(),enfa2.states.begin(),enfa2.states.end());
	set<string> newAlph = enfa1.alphabet;
	newAlph.insert(enfa2.alphabet.begin(),enfa2.alphabet.end());
	for(int i=enfa1.states.size() ; i<int(newStates.size()) ; i++){
		for(map<string,set<int>>::iterator transition = newStates.at(i).transitions.begin() ; transition!=newStates.at(i).transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+enfa1.states.size());
			}
			transition->second=newSet;
		}
	}
	newStates.at(enfa1.states.size()-1).acceptState=false;
	newStates.at(newStates.size()-1).acceptState=true;
	set<int> reach1={int(enfa1.states.size())};
	newStates.at(enfa1.states.size()-1).transitions[""]=reach1;
	eNFA neweNFA(newStates,newAlph);
	return neweNFA;
}

eNFA operator+(const eNFA &enfa1, const eNFA &enfa2) {

	std::vector<State<string,set<int>>> states;
	int beginIndex = enfa1.states.size()+1;

	State<string,set<int>> headState;
	std::set<int> reach1;
	reach1.insert(1);
	reach1.insert(beginIndex);
	headState.transitions[""]=reach1;
	states.push_back(headState);

	for(vector<State<string,set<int>>>::const_iterator state = enfa1.states.begin() ; state!=enfa1.states.end() ; state++){
		State<string,set<int>> newState;
		for(map<string,set<int>>::const_iterator transition = state->transitions.begin() ; transition!=state->transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::const_iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+1);
			}
			newState.transitions[transition->first]=newSet;
		}
		states.push_back(newState);
	}


	for(vector<State<string,set<int>>>::const_iterator state = enfa2.states.begin() ; state!=enfa2.states.end() ; state++){
		State<string,set<int>> newState;
		for(map<string,set<int>>::const_iterator transition = state->transitions.begin() ; transition!=state->transitions.end() ; transition++){
			set<int> newSet;
			for(set<int>::const_iterator index = transition->second.begin() ; index!=transition->second.end() ; index++){
				newSet.insert((*index)+beginIndex);
			}
			newState.transitions[transition->first]=newSet;
		}
		states.push_back(newState);
	}

	State<string,set<int>> tailState;
	tailState.acceptState=true;
	states.push_back(tailState);

	states.at(beginIndex-1).acceptState=false;
	states.at(states.size()-2).acceptState=false;

	set<int> set2= {int(states.size())-1};
	states.at(beginIndex-1).transitions[""]=set2;
	states.at(states.size()-2).transitions[""]=set2;

	std::set<string> newAlph = enfa1.alphabet;
	newAlph.insert(enfa2.alphabet.begin(),enfa2.alphabet.end());

	eNFA neweNFA(states,newAlph);
	return neweNFA;
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

eNFA geteNFA(char token) {
	string symbool(1,token);
	set<string> alph1 = {symbool,""};
	vector<State<string,set<int>> > states1;
	states1.push_back(State<string,set<int>>());
	set<int> set1 = {1};
	states1[0].transitions[symbool] = set1;
	states1[0].acceptState = false;
	states1.push_back(State<string,set<int>>());
	eNFA enfa1(states1,alph1);
	return enfa1;
}

bool isSymbool(char symbool){
	return !(symbool == '(' || symbool == ')' || symbool == '+' || symbool == '*');
}

string setPoints(string regex){
	assert(regex.size()>0);
	string newRegex;
	for(string::iterator token = regex.begin() ; token != regex.end() ; ++token){
		if((token+1)!=regex.end()){
			if(isSymbool(*token) && (isSymbool(*(token+1)) || *(token+1) == '(') ){		//twee symbolen naast elkaar of ( na een symbool
				newRegex+=*token;
				newRegex+=".";
			}
			else if((*token) == '*' && (isSymbool(*(token+1)) || *(token+1)=='(') ){	//Als na ster een symbool of (
				newRegex+="*";
				newRegex+=".";
			}
			else if((*token) == ')' && (isSymbool(*(token+1)) || *(token+1)=='(') ){	//Als na ) een symbool of (
				newRegex+=")";
				newRegex+=".";
			}
			else{
				newRegex+=*token;
			}

		}
		else{
			newRegex+=*token;
		}


	}
	return newRegex;
}


int getPrecedence(char expression){
	if			(expression=='(' or expression==')')		return 4;
	else if		(expression=='*' )							return 3;
	else if		(expression=='.')							return 2;
	else if		(expression=='+')							return 1;
}

string infixToPostfix(string expression) {
	stack<char> astack;
	string postfix;
	for(int i=0 ; i<expression.size() ; i++){
		char symbool = expression.at(i);
		if(symbool=='('){
			astack.push(symbool);
		}
		else if(symbool== ')'){
			while(astack.top()!='('){
				postfix +=astack.top();
				astack.pop();
			}
			astack.pop();
		}
		else if(symbool=='*' or symbool=='+' or symbool=='.'){
			while( !astack.empty() && (astack.top()!='(') && (getPrecedence(symbool)<=getPrecedence(astack.top())) ){
				postfix +=astack.top();
				astack.pop();
			}
			astack.push(symbool);
		}
		else{
			postfix +=symbool;
		}
	}
	while(!astack.empty()){
		postfix +=astack.top();
		astack.pop();
	}
	return postfix;
}
eNFA regexToeNFA(std::string regexBegin) {
	stack<eNFA> astack;
	string regex = setPoints(regexBegin);
	regex = infixToPostfix(regex);
	for(string::iterator symbool = regex.begin() ; symbool!=regex.end() ; symbool++){
		char token = *symbool;
		if(token!='*' and token!='.' and token!='+'){
			astack.push(geteNFA(token));
		}
		else if(token=='*'){
			assert(astack.size()>=1);
			eNFA top = astack.top();
			astack.pop();
			astack.push(*top);
		}
		else if(token=='+'){
			assert(astack.size()>=2);
			eNFA top = astack.top();
			astack.pop();
			eNFA next = astack.top();
			astack.pop();
			astack.push(next+top);
		}
		else if(token=='.'){
			assert(astack.size()>=2);
			eNFA top = astack.top();
			astack.pop();
			eNFA next = astack.top();
			astack.pop();
			astack.push(next^top);
		}
	}
	assert(astack.size()==1);
	return astack.top();
}




