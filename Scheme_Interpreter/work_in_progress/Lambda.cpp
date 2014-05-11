#include "Lambda.h"

Lambda::Lambda() {}
Lambda::Lambda(Expression& exp, Expression& symbols,
               Environment* outer)
{
    std::list<Symbol> symbolList;
    if (symbols.getType() == List)
        for (auto& symbol: symbols.getAsList())
            symbolList.push_back(symbol.getAsSymbol());
    // if it's not a list, it should be a single symbol
    else symbolList.push_back(symbols.getAsSymbol()); 

    nArgsExpected = symbolList.size();

    f = [=](std::list<Expression>& params) -> Expression {
        Environment newEnvironment(symbolList, params, outer);
        return evaluate(exp, newEnvironment);
    };
}

Lambda::Lambda(Ftype f, int nArgsExpected)
    : f(f), nArgsExpected(nArgsExpected)
{}

Expression Lambda::operator()(std::list<Expression>& params) const
{
    if (params.size() == nArgsExpected)
        return f(params);
}
