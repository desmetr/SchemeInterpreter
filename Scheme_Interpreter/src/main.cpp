#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>
#include <stack>

#include "CharacterCategory.h"
#include "Expression.h"
#include "Environment.h"
#include "Lambda.h"
#include "DFA.h"
#include "eNFA.h"
#include "Parser.h"
#include "Evaluate.h"

void deleteTabAndExtraSpace(string& input){
	string::iterator letter = input.begin();
	int count = 0;
	while( letter != input.end()){
		if(*letter == '\t'){
			letter = input.erase(letter);
			count = 0;
		}
		else if(*letter == ' '){
			++count;
			++letter;
		}
		else{
			if(count > 1){
				letter = input.erase((letter-(count-1)),letter);
			}
			else{
				++letter;
			}
			count = 0;
		}
	}
}


bool checkMatchingParen(string input){
	stack<char> stackk;
	for(char letter: input){
		if(letter == '('){
			stackk.push(letter);
		}
		else if(letter == ')'){
			stackk.pop();
		}
	}
	return stackk.empty();
}
void initGlobalEnvironment(Environment& global) {
	// +
	Lambda add(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) + *std::next(params.begin());
	}), 2);
	global.addSymbol("+", add);

	// -
	Lambda subtract(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) - *std::next(params.begin());
	}), 2);
	global.addSymbol("-", subtract);

	// *
	Lambda multiply(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) * *std::next(params.begin());
	}), 2);
	global.addSymbol("*", multiply);

	// /
	Lambda divide(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) / *std::next(params.begin());
	}), 2);
	global.addSymbol("/", divide);

	// <
	Lambda isSmaller(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) < *std::next(params.begin());
	}), 2);
	global.addSymbol("<", isSmaller);

	// >
	Lambda isGreater(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) > *std::next(params.begin());
	}), 2);
	global.addSymbol(">", isGreater);

	// =
	Lambda isEqual(Ftype([](std::list<Expression>& params) {
		return *(params.begin()) == *std::next(params.begin());
	}), 2);
	global.addSymbol("=", isEqual);

	// cons: ~push_front - (cons 1 (list 2 3)) -> (1 2 3)
	Lambda cons(Ftype([](std::list<Expression>& params) {
		std::list<Expression> result = params.back().getAsList();
		result.push_front(params.front());
		return Expression(result);
	}), 2);
	global.addSymbol("cons", cons);

	// car: eerste element van een lijst opvragen - (car (list 1 2 3)) -> 1
	Lambda car(Ftype([](std::list<Expression>& params) {
		return params.front().getAsList().front();
	}), 1);
	global.addSymbol("car", car);

	// cdr: alles behalve eerste element van een lijst - (cdr (list 1 2 3)) -> (2 3)
	Lambda cdr(Ftype([](std::list<Expression>& params) {
		return Expression(std::list<Expression>(
				std::next(params.front().getAsList().begin()),
				params.front().getAsList().end()));
	}), 1);
	global.addSymbol("cdr", cdr);

	// length
	Lambda getLength(Ftype([](std::list<Expression>& params) {
		return Expression(int(params.front().getAsList().size()));
	}), 1);
	global.addSymbol("length", getLength);
}

int main(int argc, char* argv[]) {

	std::shared_ptr<Environment> global_ptr(new Environment);
	initGlobalEnvironment(*global_ptr);

	if (argc > 1)	
	{
		for(int i = 1; i < argc; ++i){
			Expression exp;
			ifstream infile(argv[i]);

			if (infile.is_open() and infile.good())
			{
				while (true)
				{
					string input = "";
					string line = "";
					bool a = false;
					bool emptyLine = false;
					do{
						if(!getline(infile, line)){
							a = true;
							break;
						}
						input += line;

					} while(!checkMatchingParen(input));

					if (a) break;
					if(line == ""){
						continue;
					}
					deleteTabAndExtraSpace(input);
					try
					{
						parse(exp, input);
					}
					catch (const std::runtime_error e)
					{
						std::cerr << "Parsing error: " << e.what() << std::endl;
						continue;
					}
					try
					{
						evaluate(exp, global_ptr).print();
					}
					catch (const std::exception e)
					{
						std::cerr << "Evaluation error: " << e.what() << std::endl;
						continue;
					}
				}
			}
			else
			{
				cout << "Failed to open file." << endl;
			}
		}
	}
	while (true) {
		std::cout << std::endl << "> ";
		string input = "";
		string line = "";
		do{

			getline(cin, line);
			input += line;

		}while(!checkMatchingParen(input));
		if(line == ""){
			continue;
		}
		deleteTabAndExtraSpace(input);
		input.push_back(' ');

		Expression exp;
		try {
			parse(exp, input);
		} catch (const std::runtime_error e) {
			std::cerr << "Parsing error: " << e.what() << std::endl;
			continue;
		}

		//try {
		evaluate(exp, global_ptr).print();
		//} catch (const std::exception e) {
		//std::cerr << "Evaluation error: " << e.what() << std::endl;
		//continue;
		//}

	}
}
