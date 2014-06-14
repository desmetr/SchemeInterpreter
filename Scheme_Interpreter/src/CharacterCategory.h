#ifndef CHARACTER_DEFINED
#define CHARACTER_DEFINED

#include <string>
#include <algorithm>
#include <iterator>

const char ANY_LETTER = 'a';
const char ANY_DIGIT = '0';
const char OPENING_PAREN = '(';
const char CLOSING_PAREN = ')';
const char DECIMAL_POINT = '.';
const char OTHER = '_';

inline bool isLetter(char c) {
	return (65 <= c && c <= 90) || (97 <= c && c <= 122);
}

inline bool isDigit(char c) {
	return (48 <= c && c <= 57);
}

inline char getCharacterCategory(char c) {
	if      (isLetter(c))	return ANY_LETTER;
	else if (isDigit(c))	return ANY_DIGIT;
	else if (c == 40)		return OPENING_PAREN;
	else if (c == 41)		return CLOSING_PAREN;
	else if (c == 46)		return DECIMAL_POINT;
	else 					return OTHER;
}

#endif
