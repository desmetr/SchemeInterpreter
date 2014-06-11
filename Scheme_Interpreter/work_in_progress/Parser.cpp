/*
 * GetExpression.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: ajay
 */

#include "GetExpression.h"

bool isSymbol(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkSymbol.readUntilAccepted(input,newString)) {
		ex = Expression(newString);
		return true;
	}
	return false;
}

bool isInt(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkInteger.readUntilAccepted(input,newString)) {
		ex = Expression(stoi(newString));
		return true;
	}
	return false;
}

bool isFloat(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkFloat.readUntilAccepted(input,newString)) {
		ex = Expression(stof(newString));
		return true;
	}
	return false;
}

bool isBooleanTrue(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkBooleanTrue.readUntilAccepted(input,newString)) {
		ex = Expression(true);
		return true;
	}
	return false;
}

bool isBooleanFalse(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkBooleanFalse.readUntilAccepted(input,newString)) {
		ex = Expression(true);
		return true;
	}
	return false;
}

bool isOpeningParen(string& input) {
	string newString;
	if(standardDFAs::checkOpeningParen.readUntilAccepted(input,newString)) {
		return true;
	}
	return false;
}

bool isClosingParen(string& input) {
	string newString;
	if(standardDFAs::checkClosingParen.readUntilAccepted(input,newString)) {
		return true;
	}
	return false;
}

Expression parseList(string& input) {
	std::list<Expression> result;
	const string origInput = input;
	while (input.good()) {
		if (isClosingParen(input)) {
			input = input.substr(1);
			return Expression(result);
		}
		result.push_back(parse(input));
	}
	raise runtime_error("input + " is not a valid list");
}

Expression parseAtom(string& input) {
	typedef function<bool(Expression&,string&)> Lexer;
	const static set<Lexer> lexers {
		lexFunc(isSymbol),
		lexFunc(isInt),
		lexFunc(isFloat),
		lexFunc(isBooleanTrue),
		lexFunc(isBooleanFalse)
	};

	Expression result;
	const string origInput = input;
	for (const Lexer& lexer : lexers) {
		if (lexer(result, input)) return result;
		input = origInput;
	}
	raise runtime_error("First element of " + input + " is not a valid atom");
}

Expression parse(string& input) {
	const string origInput = input;
	if (isOpeningParen(input))
		return parseList(origInput.substr(1));
	else
		return parseAtom(origInput);
}
