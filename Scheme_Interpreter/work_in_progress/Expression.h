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
    Expression(Symbol& s);
    Expression(int i);
    Expression(double d);
    Expression(const std::list<Expression>& l);
    Expression(Lambda& l);
    Expression(const Expression& v);
    ~Expression();

    ExpressionType getType() const;
    int getAsInt() const;
    double getAsFloat() const;
    const Symbol& getAsSymbol() const;
    const Lambda& getAsFunction() const;
    const std::list<Expression>& getAsList() const;

    operator bool() const;

    Expression operator+(int i) const;
    Expression operator+(double d) const;
    Expression operator+(const Expression& e) const;
    Expression operator-(int i) const;
    Expression operator-(double d) const;
    Expression operator-(const Expression& e) const;
    Expression operator*(int i) const;
    Expression operator*(double d) const;
    Expression operator*(const Expression& e) const;
    Expression operator/(int i) const;
    Expression operator/(double d) const;
    Expression operator/(const Expression& e) const;
};

