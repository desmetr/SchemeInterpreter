#pragma once

#include <functional>
#include <list>

#include "Expression.h"
#include "Environment.h"

Expression evaluate(const Expression& exp, Environment& env); //main.cpp

typedef std::function<Expression(std::list<Expression>&)> Ftype;

class Lambda 
{
    Ftype f;
    int nArgsExpected;

public:
    Lambda();
    Lambda(Expression& exp, Expression& symbols,
           Environment* outer);
    Lambda(Ftype f, int nArgsExpected);

    Expression operator()(std::list<Expression>& params) const;
};
