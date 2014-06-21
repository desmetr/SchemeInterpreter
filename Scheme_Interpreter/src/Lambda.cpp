#include "Lambda.h"

Lambda::Lambda() {}
Lambda::Lambda(Expression& exp, Expression& symbols,
               std::shared_ptr<Environment> outer)
{
    std::list<Symbol> symbolList;
    if (symbols.getType() == List)
        for (auto& symbol: symbols.getAsList())
            symbolList.push_back(symbol.getAsSymbol());
    // if it's not a list, it should be a single symbol
    else symbolList.push_back(symbols.getAsSymbol()); 

    nArgsExpected = symbolList.size();

    f = [=](std::list<Expression>& params) -> Expression {
        std::shared_ptr<Environment> newEnvptr
            (new Environment(symbolList, params, outer));
        return evaluate(exp, newEnvptr);
    };
}

Lambda::Lambda(Ftype f, int nArgsExpected)
    : f(f), nArgsExpected(nArgsExpected)
{}

Expression Lambda::operator()(std::list<Expression>& params) const
{
    if (params.size() == nArgsExpected)
        return f(params);
    else throw std::invalid_argument("Wrong number of arguments");
}
