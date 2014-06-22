#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>

#include "CharacterCategory.h"
#include "Expression.h"
#include "Environment.h"
#include "Lambda.h"
#include "DFA.h"
#include "eNFA.h"
#include "Parser.h"
#include "Evaluate.h"

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

	// cons
	Lambda cons(Ftype([](std::list<Expression>& params) {
		std::list<Expression> result = params.back().getAsList();
		result.push_front(params.front());
		return Expression(result);
	}), 2);
	global.addSymbol("cons", cons);
	
	// car
	Lambda car(Ftype([](std::list<Expression>& params) {
		return params.front().getAsList().front();
	}), 1);
	global.addSymbol("car", car);

	// cdr
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
		Expression exp;
		ifstream infile(argv[1]);

		if (infile.is_open() and infile.good())
		{
			string input = "";
			while (getline(infile, input))
			{
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
	while (true) {
		std::cout << std::endl << "> ";
		string input;
		getline(cin, input);
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
