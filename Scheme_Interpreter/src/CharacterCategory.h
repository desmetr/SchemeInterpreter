#ifndef CHARACTER_DEFINED
#define CHARACTER_DEFINED

#include <string>
#include <set>

const char ANY_LETTER = 'a';
const char ANY_DIGIT = '0';
const char ANY_WHITESPACE = ' ';
const char OPENING_PAREN = '(';
const char CLOSING_PAREN = ')';
const char DECIMAL_POINT = '.';
const char OTHER = '_';

const std::set<char> defaultAlphabet {
	ANY_LETTER, ANY_DIGIT, ANY_WHITESPACE, OPENING_PAREN, 
	CLOSING_PAREN, DECIMAL_POINT, OTHER
};

inline bool isLetter(char c) {
	return (65 <= c && c <= 90) || (97 <= c && c <= 122);
}

inline bool isDigit(char c) {
	return (48 <= c && c <= 57);
}

inline bool isWhitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n';
}

inline char getCharacterCategory(char c) {
	if      (isLetter(c))		return ANY_LETTER;
	else if (isDigit(c))		return ANY_DIGIT;
	else if (isWhitespace(c))	return ANY_WHITESPACE;
	else if (c == '(')			return OPENING_PAREN;
	else if (c == ')')			return CLOSING_PAREN;
	else if (c == '.')			return DECIMAL_POINT;
	else 						return OTHER;
}

#endif
