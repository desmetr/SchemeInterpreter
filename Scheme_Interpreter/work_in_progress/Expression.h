#pragma once

#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <list>
#include <stdexcept>
#include <functional>

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
    Expression(Lambda& l);
    Expression(const Expression& v);
    ~Expression();

    ExpressionType getType() const;
    operator bool() const;
    Expression operator+(int i) const;
    Expression operator+(double d) const;
    Expression operator+(const Expression& e) const;
    int getAsInt() const;
    double getAsFloat() const;
    const Symbol& getAsSymbol() const;
    const Lambda& getAsFunction() const;
    const std::list<Expression>& getAsList() const;
};

