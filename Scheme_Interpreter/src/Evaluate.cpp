#include "Evaluate.h"

Expression evaluate(const Expression& exp, std::shared_ptr<Environment> envptr) 
{
	if (exp.getType() == Sym) // variable reference
		return envptr->find(exp.getAsSymbol());

	else if (exp.getType() != List) // constant literal
		return exp;

	auto expAsList = exp.getAsList(); // std::list<Expression>&
	auto expIt = expAsList.begin();
	if (expIt->getType() == Sym) {
		if (expIt->getAsSymbol() == "quote") // (quote exp)
			return *(++expIt);

		else if (expIt->getAsSymbol() == "if") { // (if test conseq alt)
			auto& test = *(++expIt); auto& conseq= *(++expIt);
			auto& alt= *(++expIt);
			return evaluate((evaluate(test,envptr) ? conseq : alt), envptr);
		}
		else if (expIt->getAsSymbol() == "set!") { // (set! symbol value)
			auto& symbol = *(++expIt); auto& value = *(++expIt);
			envptr->setSymbol(symbol.getAsSymbol(), evaluate(value, envptr));
			return Expression();
		}
		else if (expIt->getAsSymbol() == "let") { // (let symbol value exp)
			auto& symbol = *(++expIt); auto& value = *(++expIt);
			auto& exp = *(++expIt);
			std::shared_ptr<Environment> local
				(new Environment({symbol.getAsSymbol()}, {evaluate(value, envptr)}, envptr));
			return evaluate(exp, local);
		}
		else if (expIt->getAsSymbol() == "define") { // (define symbol value)
			auto& symbol = *(++expIt); auto& value = *(++expIt);
			envptr->addSymbol(symbol.getAsSymbol(), evaluate(value, envptr));
			return Expression();
		}
		else if (expIt->getAsSymbol() == "lambda") { // (lambda (paramSym*) opExp)
			auto& paramSymbols = *(++expIt); auto& lambdaExp = *(++expIt);
			Lambda l(lambdaExp, paramSymbols, envptr);
			return Expression(l);
		}
		else if (expIt->getAsSymbol() == "list") { // (list e1 e2 ... en)
			std::list<Expression> result;
			while (++expIt != expAsList.end())
				result.push_back(evaluate(*expIt, envptr));
			return Expression(result);
		}
		else if (expIt->getAsSymbol() == "begin") {
			while (++expIt != std::prev(expAsList.end()))
				evaluate(*expIt, envptr);
			return evaluate(expAsList.back(), envptr);
		}
	}
	// evaluate as a function call ~ (func arg1 arg2 ... argN)
	std::list<Expression> exps;
	for (auto& e: expAsList)
		exps.push_back(evaluate(e, envptr));
	Expression function = exps.front();
	exps.pop_front();
	return function.getAsFunction()(exps);
}
