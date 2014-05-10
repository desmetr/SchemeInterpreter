#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>

enum ExpressionType {Sym, Int, Float, Boolean, List, Function, None};
typedef std::string Symbol;

class Environment;
class Lambda;

class Expression
{
    ExpressionType type;
    void* expptr;

public:
    Expression();
    Expression(int i);
    Expression(double d);
    Expression(const Expression& v);
    ~Expression();

    operator bool() const;
    const Symbol& getAsSymbol() const;
    const std::list<Expression>& getAsList() const;
    const Lambda& getAsLambda() const;

    // defined in main.cpp
    friend Expression evaluate(const Expression& exp, Environment& env);
};

#include "Environment.h"

class Lambda
{
    Expression exp;
    std::list<Symbol> paramSyms;
    Environment* outer;
    int nArgsRequired;

public:
    Lambda(const Expression& exp, const std::list<Symbol>& paramSyms, 
           Environment* outer);

    Expression operator() (const std::list<Expression>& params);
};
