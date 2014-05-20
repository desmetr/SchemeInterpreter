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
			testNumber << "test"<<i;
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
				testRegex_Enfa(conf,testNumber.str());
			}
		}
	}
}

void testMinimize(const ini::Configuration& conf, const string testNr ) {
	string dfaNr = conf[testNr]["input"].as_string_or_die();
	DFA dfa = getDFA(conf,dfaNr);
	dfa.minimize();
	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<dfa;
	ofStream3.close();
	string command = "dot -Tjpg " + testNr + " -O";
	system(command.c_str());

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
	string command = "dot -Tjpg " + testNr + " -O";
	system(command.c_str());

	ofstream ofStream4;
	ofStream4.open("Dfa1");
	ofStream4<<dfa1;
	ofStream4.close();
	string command2 = "dot -Tjpg Dfa1 -O";
	system(command2.c_str());

	ofstream ofStream5;
	ofStream5.open("Dfa2");
	ofStream5<<dfa2;
	ofStream5.close();
	string command3 = "dot -Tjpg Dfa2 -O";
	system(command3.c_str());
}

void testRegex_Enfa(const ini::Configuration& conf, string testNr) {
	string regex = conf[testNr]["regex"].as_string_or_die();
	eNFA enfa = regexToeNFA(regex);;
	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<enfa;
	ofStream3.close();
	string command = "dot -Tjpg " + testNr + " -O";
	system(command.c_str());

	DFA dfa2 = enfa.modSubCnstr();
	dfa2.minimize();

	ofstream ofStream2;
	ofStream2.open("msc");
	ofStream2<<dfa2;
	ofStream2.close();
	string command2 = "dot -Tjpg msc -O";
	system(command2.c_str());


}

void testMod_Subset(const ini::Configuration& conf, string testNr) {
	string eNfaNr = conf[testNr]["input"].as_string_or_die();
	eNFA enfa = getEnfa(conf,eNfaNr);


	ofstream ofStream1;
	ofStream1.open("enfa1");
	ofStream1<<enfa;
	ofStream1.close();
	string command1 = "dot -Tjpg enfa1 -O";
	system(command1.c_str());

	DFA dfa = enfa.modSubCnstr();

	ofstream ofStream3;
	ofStream3.open(testNr.c_str());
	ofStream3<<dfa;
	ofStream3.close();
	string command2 = "dot -Tjpg " + testNr + " -O";
	system(command2.c_str());
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
		nrState<<"state"<<i<<"Met";
		for(char sym:alphabet){
			try{
				stringstream nrState;
				nrState<<"state"<<i<<"Met";
				nrState<<sym;
				int index = conf[dfaNr][nrState.str()].as_int_or_die();
				state.transitions[sym]=index;
			}
			catch(ini::ParseException& ex){
				cerr<<"Invalid "<<dfaNr<<". Niet alle symbolen per staat zijn gegeven. "<<nrState.str()<<endl;
			}
		}
		states.push_back(state);
	}
	return DFA(states,alphabet);
}

eNFA getEnfa(const ini::Configuration& conf, string eNFANr) {
	int nrStates 	=	 conf[eNFANr]["nStates"].as_int_or_die();
	int nrSymbool 	=	 conf[eNFANr]["nChars"].as_int_or_die();
	vector<int> acceptStates	=	 conf[eNFANr]["acceptStates"].as_int_tuple_or_die();
	set <string> alphabet;
	for(int i=0 ; i<nrSymbool ; i++){
		stringstream nrChar;
		nrChar<<"char"<<i;
		string symbool = conf[eNFANr][nrChar.str()].as_string_or_die();
		if(symbool.size()==1 || symbool.size()==0){
			alphabet.insert(symbool);
		}
		else{
			cerr<<"Invaid alphabet "<<eNFANr<<endl;
		}
	}
	vector<State<string,set<int>>> states;
	for(int i=0 ; i<nrStates ; i++){
		State<string,set<int>> state;
		if(find(acceptStates.begin(),acceptStates.end(),i)!=acceptStates.end()){
			state.acceptState=true;
		}
		stringstream nrState;
		nrState<<"state"<<i<<"Met";
		for(string sym:alphabet){
			try{
				stringstream nrState;
				nrState<<"state"<<i<<"Met";
				nrState<<sym;
				if(conf[eNFANr][nrState.str()].exists()){
					vector<int> index = conf[eNFANr][nrState.str()].as_int_tuple_or_die();
					set<int> indices;
					for(int i:index){
						indices.insert(i);
					}
					state.transitions[sym]=indices;
				}
			}
			catch(ini::ParseException& ex){
				cerr<<"Invalid "<<eNFANr<<". Niet alle symbolen per staat zijn gegeven. "<<nrState.str()<<endl;
			}
		}
		states.push_back(state);
	}
	return eNFA(states,alphabet);

}
