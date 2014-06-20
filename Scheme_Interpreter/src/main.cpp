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

// Interpretatie (uitgezonderd de parser/lexer) gebaseerd op de tutorial
//      "(How to Write a (Lisp) Interpreter (in Python))"
//          (norvig.com/lispy.html)

// (define y 4.5)
// y
// (define x 7)
//
Expression evaluate(const Expression& exp, Environment& env) {
	if (exp.getType() == Sym) // variable reference
		return env.find(exp.getAsSymbol());

	else if (exp.getType() != List) // constant literal
		return exp;

	auto expAsList = exp.getAsList(); // std::list<Expression>&
	auto expIt = expAsList.begin();
	if (expIt->getType() == Sym) {
		if (expIt->getAsSymbol() == "quote") // (quote exp)
			return *(++expIt);

		else if (expIt->getAsSymbol() == "if") { // (if test conseq alt)
			auto& test = *(++expIt); auto& conseq= *(++expIt);
			auto& alt= *(++expIt);
			return evaluate((evaluate(test,env) ? conseq : alt), env);
		}
		else if (expIt->getAsSymbol() == "set!") { // (set! symbol value)
			auto& symbol = *(++expIt); auto& value = *(++expIt);
			env.setSymbol(symbol.getAsSymbol(), evaluate(value, env));
		}
		else if (expIt->getAsSymbol() == "define") { // (define symbol value)
			auto& symbol = *(++expIt); auto& value = *(++expIt);
			env.addSymbol(symbol.getAsSymbol(), evaluate(value, env));
		}
		else if (expIt->getAsSymbol() == "lambda") { // (lambda (paramSym*) opExp)
			auto& paramSymbols = *(++expIt); auto& lambdaExp = *(++expIt);
			Lambda l(lambdaExp, paramSymbols, &env);
			return Expression(l);
		}
		else if (expIt->getAsSymbol() == "list") { // (list e1 e2 ... en)
			std::list<Expression> result;
			while (++expIt != expAsList.end())
				result.push_back(evaluate(*expIt, env));
			return Expression(result);
		}
	}
	// TODO: BEGIN
	// (define square (lambda a (* a a))) -> 0 (square gedefineerd)
	// (define multiplyBySelf square)
	// (multiplyBySelf (+ 3 4)) -> (square 7)
	else {
		std::list<Expression> exps;
		for (auto& e: expAsList)
			exps.push_back(evaluate(e, env));
		Expression function = exps.front();
		exps.pop_front();
		//std::cout << "function: ";
		//function.print(); std::cout << std::endl;
		return function.getAsFunction()(exps);
	}
	return Expression();
}

Environment initGlobalEnvironment() {
	Environment global;

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

	return global;
}

int main() {
	Environment global = initGlobalEnvironment();
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
			evaluate(exp, global).print();
		//} catch (const std::exception e) {
			//std::cerr << "Evaluation error: " << e.what() << std::endl;
			//continue;
		//}
	}
}
