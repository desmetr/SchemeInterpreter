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
