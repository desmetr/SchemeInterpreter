#pragma once

#include <functional>
#include <list>
#include <memory>

#include "Expression.h"
#include "Environment.h"
#include "Evaluate.h"

Expression evaluate(const Expression& exp, Environment& env); //main.cpp

typedef std::function<Expression(std::list<Expression>&)> Ftype;

class Lambda 
{
    Ftype f;
    int nArgsExpected;

public:
    Lambda();
    Lambda(Expression& exp, Expression& symbols,
           std::shared_ptr<Environment> outer);
    Lambda(Ftype f, int nArgsExpected);

    Expression operator()(std::list<Expression>& params) const;
};
