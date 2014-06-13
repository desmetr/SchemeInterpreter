/*
 * GetExpression.h
 *
 *  Created on: Jun 6, 2014
 *      Author: ajay
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <map>
#include "Expression.h"
#include "../src/DFA.h"
#include "../src/eNFA.h"
#include <string.h>
using namespace std;


bool isSymbol(Expression& ex, string& input);
bool isInt(Expression& ex, string& input);
bool isFloat(Expression& ex, string& input);
bool isBooleanTrue(Expression& ex, string& input);
bool isBooleanFalse(Expression& ex, string& input);
bool isOpeningParen(string& input);
bool isClosingParen(string& input);

string getAlphabet();
void makeDFA(string regex, string allSymboles, DFA& dfa);

Expression parseList(string& input);
Expression parseAtom(string& input);
Expression parse(string& input);




#endif /* PARSER_H_ */
