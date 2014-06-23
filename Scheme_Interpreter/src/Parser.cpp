#include "Parser.h"

namespace standardDFAs {
	string delimiters = "( +\\(+\\))";
	string anyWhitespace = "( +\t+\n)*";
	string symbol = "(a+_)(a+_+0)*" + delimiters;
	string floatt = "00*\\.00*" + delimiters;
	string integer = "00*" + delimiters;
	string openingParen = anyWhitespace + "\\(";
	string closingParen = anyWhitespace + "\\)";
	
	DFA checkSymbol(regexToDFA(symbol, defaultAlphabet));
	DFA checkInteger(regexToDFA(integer, defaultAlphabet));
	DFA checkFloat(regexToDFA(floatt, defaultAlphabet));
	DFA checkOpeningParen(regexToDFA(openingParen, defaultAlphabet));
	DFA checkClosingParen(regexToDFA(closingParen, defaultAlphabet));
	DFA whitespaceTrimmer(regexToDFA(anyWhitespace, defaultAlphabet));
};

StrIt isSymbol(Expression& ex, StrIt begin, StrIt end) {
	auto it = standardDFAs::checkSymbol.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
	if (it != end) {
		Symbol exval(begin, it);
		ex.setType(Sym);
		ex.setVal(exval);
	}
	return it;
}

StrIt isInt(Expression& ex, StrIt begin, StrIt end) {
	auto it = standardDFAs::checkInteger.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
	if (it != end) {
		ex.setType(Int);
		ex.setVal(stoi(string(begin,it)));
	}
	return it;
}

StrIt isFloat(Expression& ex, StrIt begin, StrIt end) {
	auto it = standardDFAs::checkFloat.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
	if (it != end) {
		ex.setType(Float);
		ex.setVal(stod(string(begin,it)));
	}
	return it;
}

StrIt isOpeningParen(StrIt begin, StrIt end) {
	auto it = standardDFAs::checkOpeningParen.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
	return it;
}

StrIt isClosingParen(StrIt begin, StrIt end) {
	auto it = standardDFAs::checkClosingParen.readUntilAccepted(
		TransIt(begin, getCharacterCategory), end).getIt();
	return it;
}

StrIt trimWhitespace(StrIt begin, StrIt end) {
	auto it = standardDFAs::whitespaceTrimmer.readUntilRejected(
		TransIt(begin, getCharacterCategory), end).getIt();
	return it;
}

StrIt parseList(Expression& ex, StrIt begin, StrIt end) {
	ex.setType(List);
	std::list<Expression> result;
	auto it = trimWhitespace(begin, end);
	while (it != end) {
		auto newIt = isClosingParen(it, end);
		if (newIt != end) { // If closing paren detected
			ex.setVal(result); // Wrap this function up
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
		Lexer(&isFloat)
	};

	for (const auto& lexer : lexers) {
		try {
			auto it = lexer(ex, begin, end);
			if (it != end) // If the lexer accepts the string
				return it; 
					// Return an iterator to the character triggering the 
					//  accept state
		} catch (const std::out_of_range e) {
			std::cerr << "parseAtom: unrecognised symbol" << std::endl;
		}
	}
	// If the for loop failed, the string is invalid.
	throw runtime_error("parseAtom asked to parse invalid atom");
}

StrIt parse(Expression& ex, StrIt begin, StrIt end) {
	auto trueBegin = trimWhitespace(begin, end);
	auto it = isOpeningParen(trueBegin, end);
	if (it != end)
		return parseList(ex, next(it), end); 
			// Start parsing from after opening paren
	else
		return parseAtom(ex, trueBegin, end);
}

Expression parse(Expression& result, string& s) {
	parse(result, s.begin(), s.end());
	return result;
}
