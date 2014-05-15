/*
 * readInput.cpp
 *
 *  Created on: May 15, 2014
 *      Author: ajay
 */
#include "readInput.h"

void input(int argc, char const* argv[]){
	for(int i = 1; i < argc; ++i)
	{
		ini::Configuration conf;
		try
		{
			std::ifstream fin(argv[i]);
			fin >> conf;
			fin.close();
		}
		catch(ini::ParseException& ex)
		{
			std::cerr << "Error parsing file: " << argv[i] << ": " << ex.what() << std::endl;
			continue;
		}
		int nrTest = conf["General"]["nTests"].as_int_or_die();
		for(int i=0 ; i<nrTest ; ++i){
			stringstream testNumber;
			testNumber << "Test"<<i;
			string type = conf[testNumber.str()]["type"].as_string_or_die();
			if(type=="minimization"){
				testMinimize(conf,testNumber.str());
			}
			else if(type=="product"){
				testProduct(conf,testNumber.str());
			}
			else if(type=="msc"){
				testMod_Subset(conf,testNumber.str());
			}
			else if(type=="regex_to_enfa"){
				testMod_Subset(conf,testNumber.str());
			}
		}
	}
}

void testMinimize(const ini::Configuration& conf, const string testNr ) {
	string dfaNr = conf[testNr]["input0"].as_string_or_die();
	DFA dfa = getDFA(conf,dfaNr);
	dfa.minimize();
	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<dfa;
	ofStream3.close();

}

void testProduct(const ini::Configuration& conf, string testNr) {
	string dfaNr1 = conf[testNr]["input0"].as_string_or_die();
	string dfaNr2 = conf[testNr]["input1"].as_string_or_die();
	DFA dfa1 = getDFA(conf,dfaNr1);
	DFA dfa2 = getDFA(conf,dfaNr2);
	DFA dfa3 = dfa1*dfa2;
	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<dfa3;
	ofStream3.close();
}

void testRegex_Enfa(const ini::Configuration& conf, string testNr) {
	string regex = conf[testNr]["regex"].as_string_or_die();
	eNFA enfa = regexToeNFA(regex);
	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<enfa;
	ofStream3.close();
}

void testMod_Subset(const ini::Configuration& configuration, string testNr) {
}

DFA getDFA(const ini::Configuration& conf, string dfaNr) {
	int nrStates 	=	 conf[dfaNr]["nStates"].as_int_or_die();
	int nrSymbool 	=	 conf[dfaNr]["nChars"].as_int_or_die();
	vector<int> acceptStates	=	 conf[dfaNr]["acceptStates"].as_int_tuple_or_die();
	set <char> alphabet;
	for(int i=0 ; i<nrSymbool ; i++){
		stringstream nrChar;
		nrChar<<"char"<<i;
		string symbool = conf[dfaNr][nrChar.str()].as_string_or_die();
		if(symbool.size()==1){
			alphabet.insert(symbool[0]);
		}
		else{
			cerr<<"Invaid alphabet "<<dfaNr<<endl;
		}
	}
	vector<State<char,int>> states;
	for(int i=0 ; i<nrStates ; i++){
		State<char,int> state;
		if(find(acceptStates.begin(),acceptStates.end(),i)!=acceptStates.end()){
			state.acceptState=true;
		}

		stringstream nrState;
		nrState<<"state_"<<i<<"_";
		for(char sym:alphabet){
			try{
				nrState<<sym;
				int index = conf[dfaNr][nrState.str()].as_int_or_die();
				state.transitions[sym]=index;
			}
			catch(ini::ParseException& ex){
				cerr<<"Invalid "<<dfaNr<<". Niet alle symbolen per staat is gegeven. "<<nrState.str()<<endl;
			}
		}
		states.push_back(state);
	}
	return DFA(states,alphabet);
}

eNFA getEnfa(const ini::Configuration& configuration, string eNFANr) {

}
