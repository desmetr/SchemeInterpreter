/*
 * GetExpression.h
 *
 *  Created on: Jun 6, 2014
 *      Author: ajay
 */

#ifndef PARSER_H_
#define PARSER_H_
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <functional>
#include "Expression.h"
#include "DFA.h"
#include "eNFA.h"
#include "CharacterCategory.h"

using namespace std;

typedef string::iterator StrIt;
typedef TransformingIterator< StrIt , function<char(char)> > TransIt;

// If true, these functions return a StrIt pointing to the character that 
//  triggered them into accepting the string (whitespace or parenthesis).
StrIt isSymbol(Expression& ex, StrIt begin, StrIt end);
StrIt isInt(Expression& ex, StrIt begin, StrIt end);
StrIt isFloat(Expression& ex, StrIt begin, StrIt end);

// If true, returns a StrIt on the opening paren, if false, returns end;
StrIt isOpeningParen(StrIt begin, StrIt end);

// If true, returns a StrIt on the closing paren, if false, returns end;
StrIt isClosingParen(StrIt begin, StrIt end);

void makeDFA(string regex, string allSymboles, DFA& dfa);

// Parses the first element of the string, assuming that this is a list.
StrIt parseList(Expression& ex, StrIt begin, StrIt end);

// Parses the first element of the string, assuming that this is an atom.
StrIt parseAtom(Expression& ex, StrIt begin, StrIt end);

// Parses the entire string and fills in an expression object.
StrIt parse(Expression& ex, StrIt begin, StrIt end);

// Parses the entire string and returns an expression object.
Expression parse(string s);

#endif /* PARSER_H_ */
