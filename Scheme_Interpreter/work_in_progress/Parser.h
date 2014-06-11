/*
 * GetExpression.h
 *
 *  Created on: Jun 6, 2014
 *      Author: ajay
 */

#ifndef PARSER_H_
#define PARSER_H_
using namespace std;
#include <map>
#include "Expression.h"
#include "../src/DFA.h"


bool isSymbol(Expression& ex, string& input);
bool isInt(Expression& ex, string& input);
bool isFloat(Expression& ex, string& input);
bool isBooleanTrue(Expression& ex, string& input);
bool isBooleanFalse(Expression& ex, string& input);
bool isOpeningParen(string& input);
bool isClosingParen(string& input);

Expression parseList(string& input);
Expression parseAtom(string& input);
Expression parse(string& input);

namespace standardDFAs{
	DFA checkSymbol;
	DFA checkInteger;
	DFA checkFloat;
	DFA checkBooleanTrue;
	DFA checkBooleanFalse;
	DFA checkOpeningParen;
	DFA checkClosingParen;
};




#endif /* PARSER_H_ */
