#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>

#include "Expression.h"
#include "Environment.h"
#include "Lambda.h"

// Interpretatie (uitgezonderd de parser/lexer) gebaseerd op de tutorial
//      "(How to Write a (Lisp) Interpreter (in Python))"
//          (norvig.com/lispy.html)

Expression evaluate(const Expression& exp, Environment& env) {
    if (exp.getType() == Sym) // variable reference
        return env.find(exp.getAsSymbol());
   
    else if (exp.getType() != List) // constant literal
        return exp;
   
    auto expAsList = exp.getAsList(); // std::list<Expression>&
    auto expIt = expAsList.begin();
    // ASSUMING FIRST ELEMENT IS A SYMBOL (TODO ACTUALLY CHECKING THAT)
    if (expIt->getAsSymbol() == "quote") // (quote exp)
        return *(++expIt);
   
    else if (expIt->getAsSymbol() == "if") { // (if test conseq alt)
        auto& test = *(++expIt); auto& conseq= *(++expIt);
        auto& alt= *(++expIt);
        return evaluate((evaluate(test,env) ? conseq : alt), env);
    }
    else if (expIt->getAsSymbol() == "set!") { // (set! symbol value)
        auto& symbol = *(++expIt); auto& value = *(++expIt);
        env.setSymbol(symbol.getAsSymbol(), evaluate(value, env));
    } 
    else if (expIt->getAsSymbol() == "define") { // (define symbol value)
        auto& symbol = *(++expIt); auto& value = *(++expIt);
        env.addSymbol(symbol.getAsSymbol(), evaluate(value, env));
    }
    else if (expIt->getAsSymbol() == "lambda") { // (lambda (paramSym*) opExp)
        auto& paramSymbols = *(++expIt); auto& lambdaExp = *(++expIt);
        Lambda l(lambdaExp, paramSymbols, &env);
        return Expression(l);
    }
    // TODO: BEGIN
    
    else {
        std::list<Expression> exps;
        for (auto& e: expAsList)
            exps.push_back(evaluate(e, env));
        Expression function = exps.front();
        exps.pop_front();
        return function.getAsFunction()(exps);
    }
    return Expression();
}

Environment initGlobalEnvironment() {
    Environment global;
    Lambda add(Ftype([](std::list<Expression>& params) {
        return *(params.begin()) + *std::next(params.begin());
    }), 2);
    global.addSymbol("+", add);
    return global;
}

int main() {
    Environment global = initGlobalEnvironment();
    //Expression sumFunc("+");
    Expression three(3);
    std::cout << evaluate(three, global).getAsInt() << std::endl;
    Expression four(4);
    std::cout << evaluate(four, global).getAsInt() << std::endl;
}
