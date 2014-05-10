#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>

#include "Expression.h"
#include "Environment.h"

// Interpretatie (uitgezonderd de parser/lexer) gebaseerd op de tutorial
//      "(How to Write a (Lisp) Interpreter (in Python))"
//          (norvig.com/lispy.html)

// friend function of Expression
Expression evaluate(const Expression& exp, Environment& env) {
    if (exp.type == Sym) // variable reference
        return env.find(exp.getAsSymbol());
   
    else if (exp.type != List) // constant literal
        return exp;
   
    auto expAsList = exp.getAsList(); // std::list<Expression>&
    auto expIt = expAsList.begin();
    if (expIt->type != Sym) // (proc exp*)
        return Expression();//TODO
   
    else if (expIt->getAsSymbol() == "quote")
        return *(++expIt);
   
    else if (expIt->getAsSymbol() == "if") {
        auto& test = *(++expIt); auto& conseq= *(++expIt);
        auto& alt= *(++expIt);
        return evaluate((evaluate(test,env) ? conseq : alt), env);
         
    }
    else if (expIt->getAsSymbol() == "set!") {
        auto& symbol = *(++expIt); auto& value = *(++expIt);
        env.setSymbol(symbol.getAsSymbol(), evaluate(value, env));
    } 
    else if (expIt->getAsSymbol() == "define") {
        auto& symbol = *(++expIt); auto& value = *(++expIt);
        env.addSymbol(symbol.getAsSymbol(), evaluate(value, env));
    }
    else if (expIt->getAsSymbol() == "lambda") {
        auto& paramSym = *(++expIt); auto& opExp = *(++expIt);
    }
}

int main() {}
