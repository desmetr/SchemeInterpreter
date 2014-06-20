#pragma once

#include <map>
#include <list>
#include <utility>

#include <iostream>

#include "Expression.h"
// (define a 2)
// (lambda (x y) (+ x y)) -> lokaal [x, y] -> globaal [a, +, * ...]
class Environment 
{
    std::map<Symbol,Expression> dict;
    Environment* outer;

public:
    Environment();
    Environment(const std::list<Symbol>& symbols, 
                const std::list<Expression>& values,
                Environment* outer=nullptr);
    
    Expression& find(const Symbol& s);
    void addSymbol(const Symbol& s, const Expression& e);
    void setSymbol(const Symbol& s, const Expression& e);
};
