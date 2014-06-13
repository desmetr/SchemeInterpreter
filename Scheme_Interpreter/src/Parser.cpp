/*
 * GetExpression.cpp
 *
 *  Created on: Jun 6, 2014
 *      Author: ajay
 */

#include "Parser.h"

namespace standardDFAs{

	string regexAlphabet = "(a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+w+x+y+z+A+B+C+D+E+F+G+H+I+J+K+L+M+N+O+P+Q+R+S+T+U+V+W+X+Y+Z)";
	string regexNumbers = "(0+1+2+3+4+5+6+7+8+9)";
	string allSymboles = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789#().";

	string symbol = " *" + regexAlphabet + regexAlphabet + "*( +\\()";
	string BooleanTrue = " *#t( +\\()";
	string BooleanFalse = " *#f( +\\()";
	string closingParen = " *\\(";
	string openingParen = " *\\)";
	string floatt = " *" + regexNumbers + regexNumbers + "*\\." + regexNumbers + regexNumbers + "*( +\\()";
	string integer = " *" + regexNumbers + regexNumbers + "*( +\\()";

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


bool isSymbol(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkSymbol.readUntilAccepted(input,newString)) {
		cout<<"ExpressieString:"<<newString<<endl;
		ex = Expression(newString);
		return true;
	}
	return false;
}

bool isInt(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkInteger.readUntilAccepted(input,newString)) {
		cout<<"ExpressieString:"<<newString<<endl;
		ex = Expression(stoi(newString));
		return true;
	}
	return false;
}

bool isFloat(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkFloat.readUntilAccepted(input,newString)) {
		cout<<"ExpressieString:"<<newString<<endl;
		ex = Expression(stof(newString));
		return true;
	}
	return false;
}

bool isBooleanTrue(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkBooleanTrue.readUntilAccepted(input,newString)) {
		cout<<"ExpressieString:"<<newString<<endl;
		ex = Expression(true);
		return true;
	}
	return false;
}

bool isBooleanFalse(Expression& ex, string& input) {
	string newString;
	if(standardDFAs::checkBooleanFalse.readUntilAccepted(input,newString)) {
		cout<<"ExpressieString:"<<newString<<endl;
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
void makeDFA(string regex, string allSymboles,DFA& dfa){
	eNFA eNFA = regexToeNFA(regex);
	dfa = eNFA.modSubCnstr();
	dfa.addSymbols(allSymboles);
	dfa.minimize();
}

Expression parseList(string& input) {
	std::list<Expression> result;
	const string origInput = input;
	while (input.size() > 0) {
		if (isClosingParen(input)) {
			input = input.substr(1);
			return Expression(result);
		}
		result.push_back(parse(input));
	}
	throw runtime_error(input + " is not a valid list");
}

Expression parseAtom(string& input) {
	typedef function<bool(Expression&,string&)> Lexer;
	static vector<Lexer> lexers {
		Lexer(&isSymbol),
		Lexer(&isInt),
		Lexer(&isFloat),
		Lexer(&isBooleanTrue),
		Lexer(&isBooleanFalse)
	};

	Expression result;
	const string origInput = input;
	for (const Lexer& lexer : lexers) {
		if (lexer(result, input)) return result;
		input = origInput;
	}
	throw runtime_error("First element of " + input + " is not a valid atom");
}

Expression parse(string& input) {
	//const string origInput = input;
	if (isOpeningParen(input)) {
        input = input.substr(1);
		return parseList(input);
    }
	else
		return parseAtom(input);
 }

string getAlphabet() {
	return "(a+b+c+d+e+f+g+h+i+j+k+l+m+n+o+p+q+r+s+t+u+v+w+x+y+z+A+B+C+D+E+F+G+H+I+J+K+L+M+N+O+P+Q+R+S+T+U+V+W+X+Y+Z)";
}
