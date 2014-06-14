/*
 * GetExpression.cpp
 *
 *  Created on: Jun 6, 2014
 *	  Author: ajay
 */

#include "Parser.h"

namespace standardDFAs {
	string delimiters = "( +\\()";
	string anyWhitespace = " *";
	string symbol = anyWhitespace + "aa*" + delimiters;
	string closingParen = anyWhitespace + "\\(";
	string openingParen = anyWhitespace + "\\)";
	string floatt = anyWhitespace + "00*.00*" + delimiters;
	string integer = anyWhitespace + "00*" + delimiters;

	DFA checkSymbol;
//	makeDFA(symbol, allSymboles, checkSymbol);
	DFA checkInteger;
//	makeDFA(integer, allSymboles, checkInteger);
	DFA checkFloat;
//	makeDFA(floatt, allSymboles, checkFloat);
	DFA checkBooleanTrue;
//	makeDFA(BooleanTrue, allSymboles, checkBooleanTrue);
	DFA checkBooleanFalse;
//	makeDFA(BooleanFalse, allSymboles, checkBooleanFalse);
	DFA checkOpeningParen;
//	makeDFA(openingParen, allSymboles, checkOpeningParen);
	DFA checkClosingParen;
//	makeDFA(closingParen,allSymboles, checkClosingParen);

};

StrIt isSymbol(Expression& ex, StrIt begin, StrIt end) {
	return standardDFAs::checkSymbol.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
}

StrIt isInt(Expression& ex, StrIt begin, StrIt end) {
	return standardDFAs::checkInteger.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
}

StrIt isFloat(Expression& ex, StrIt begin, StrIt end) {
	return standardDFAs::checkFloat.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
}

StrIt isOpeningParen(StrIt begin, StrIt end) {
	return standardDFAs::checkOpeningParen.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
}

StrIt isClosingParen(StrIt begin, StrIt end) {
	return standardDFAs::checkClosingParen.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
}

void makeDFA(string regex, string allSymboles,DFA& dfa){
	eNFA eNFA = regexToeNFA(regex);
	dfa = eNFA.modSubCnstr();
	dfa.addSymbols(allSymboles);
	dfa.minimize();
}

StrIt parseList(Expression& ex, StrIt begin, StrIt end) {
	std::list<Expression> result;
	auto it = begin;
	while (it != end) {
		auto newIt = isClosingParen(begin, end);
		if (newIt != end) { // If closing paren detected
			ex = Expression(result); // Wrap this function up
			return next(newIt); // Return iterator past closing paren
		}
		else {
			result.push_back(Expression());
			it = parse(result.back(), it, end);
				// Set it at the character that triggered the next accept state.
		}
	}
	throw runtime_error("parseList asked to parse invalid list");
}

StrIt parseAtom(Expression& ex, StrIt begin, StrIt end) {
	typedef function<StrIt(Expression&,StrIt,StrIt)> Lexer;
	static vector<Lexer> lexers {
		Lexer(&isSymbol),
		Lexer(&isInt),
		Lexer(&isFloat),
		Lexer(&isBooleanTrue),
		Lexer(&isBooleanFalse)
	};

	for (const auto& lexer : lexers) {
		auto it = lexer(ex, begin, end);
		if (it != end) // If the lexer accepts the string
			return it; 
				// Return an iterator to the character triggering the 
				//  accept state
	}
	// If the for loop failed, the string is invalid.
	throw runtime_error("parseAtom asked to parse invalid atom");
}

StrIt parse(Expression& ex, StrIt begin, StrIt end) {
	auto it = isOpeningParen(begin, end);
	if (it != end)
		return parseList(ex, next(it), end); 
			// Start parsing from after opening paren
	else
		return parseAtom(ex, begin, end);
 }

Expression parse(string& s) {
	Expression result;
	parse(result, s.begin(), s.end());
	return result;
}

