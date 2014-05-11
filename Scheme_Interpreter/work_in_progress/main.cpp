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

// (define y 4.5)
// y
// (define x 7)
//
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
    // (define square (lambda a (* a a))) -> 0 (square gedefineerd)
    // (define multiplyBySelf square)
    // (multiplyBySelf (+ 3 4)) -> (square 7)
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
    // +
    Lambda add(Ftype([](std::list<Expression>& params) {
        return *(params.begin()) + *std::next(params.begin());
    }), 2);
    global.addSymbol("+", add);
    // -
    Lambda subtract(Ftype([](std::list<Expression>& params) {
        return *(params.begin()) - *std::next(params.begin());
    }), 2);
    global.addSymbol("-", subtract);
    // *
    Lambda multiply(Ftype([](std::list<Expression>& params) {
        return *(params.begin()) * *std::next(params.begin());
    }), 2);
    global.addSymbol("*", multiply);
    // /
    Lambda divide(Ftype([](std::list<Expression>& params) {
        return *(params.begin()) / *std::next(params.begin());
    }), 2);
    global.addSymbol("/", divide);
    return global;
}

int main() {
    Environment global = initGlobalEnvironment();
    Symbol plus = "+";
    Symbol multiply = "*";
    Symbol define = "define";
    Symbol lambda = "lambda";
    Symbol var = "var";
    Expression plusSym(plus);
    Expression multSym(multiply);
    Expression LAMBDA(lambda);
    Expression defineSym(define);
    Expression varSym(var);

    Expression three(3);
    std::cout << evaluate(three, global).getAsInt() << std::endl; // 3
    Expression four(4);
    std::cout << evaluate(four, global).getAsInt() << std::endl; // 4

    Expression sumThreeFour(std::list<Expression> { // (+ 3 4)
            plusSym, three, four});
    std::cout << evaluate(sumThreeFour, global).getAsInt() << std::endl; // 7

    Expression defineVarSeven(std::list<Expression> { // (define var (+ 3 4))
        defineSym, varSym, sumThreeFour});
    std::cout << evaluate(defineVarSeven, global) << std::endl;
    std::cout << evaluate(varSym, global).getAsInt() << std::endl; // 7

    // (define square (lambda a (* a a))
    Symbol square = "square";
    Symbol a = "a";
    Expression squareSym(square);
    Expression aSym(a);
    Expression lambdaExp(std::list<Expression> {multSym, aSym, aSym});
    evaluate(
        std::list<Expression> 
            {defineSym, squareSym, std::list<Expression> (
                {LAMBDA, aSym, std::list<Expression>(
                    {multSym, aSym, aSym})})}, global);
    // (square var)
    std::cout << evaluate(std::list<Expression> {squareSym, var}, global).getAsInt() << std::endl;
}
