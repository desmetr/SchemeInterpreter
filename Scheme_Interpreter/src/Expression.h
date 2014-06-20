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

    void setType(ExpressionType t);

    template<typename ValueType>
    void setVal(const ValueType& val) {
        expptr = (void*) new ValueType(val);
    }

    ExpressionType getType() const;
    int getAsInt() const;
    double getAsFloat() const;
    const Symbol& getAsSymbol() const;
    const Lambda& getAsFunction() const;
    const std::list<Expression>& getAsList() const;
    void print() const;

    operator bool() const;

    Expression operator+(const Expression& e) const;
    Expression operator-(const Expression& e) const;
    Expression operator*(const Expression& e) const;
    Expression operator/(const Expression& e) const;
    Expression operator<(const Expression& e) const;
    Expression operator>(const Expression& e) const;
    Expression operator==(const Expression& e) const;
};

