#pragma once

#include <cmath>
#include <algorithm>
#include <functional>
#include <utility>
#include <map>
#include <memory>
#include <set>
#include <iostream>

//herbruikt van eigen project computer graphics (Koen)

const double deg_to_rad = 0.0174532925;
const double pi = std::acos(-1.f);

template<typename UnaryOperation, typename It>
void applyToAll(UnaryOperation o, It begin, It end)
{
    for (It it = begin; it != end; it++)
        o(*it);
}

template<typename ItType, typename ItemType>
bool contains(ItType begin, ItType end, const ItemType& item)
{
    auto itemIt = std::find(begin, end, item);
    return (itemIt != end());
}

template<typename ContainerType, typename PredicateType>
bool every(ContainerType c, PredicateType p) {
    for (auto& e: c) if (!p(e)) return false;
    return true;
}

template<typename ContainerType, typename PredicateType>
bool any(ContainerType c, PredicateType p) {
    for (auto& e: c) if (p(e)) return true;
    return false;
}

template<typename ReturnType, typename ... Args>
std::function<ReturnType(Args...)> memoize(std::function<ReturnType(Args...)> f)
{
    // gebaseerd op een voorbeeld op een blog (www.slackito.com)

    std::shared_ptr<std::map<std::tuple<Args...>, ReturnType> > memos 
        (new std::map<std::tuple<Args...>, ReturnType>);
    return std::function<ReturnType(Args...)> ([=](Args... args) {
        std::tuple<Args... > argtuple(args...);
        if (memos->find(argtuple) == memos->end())
            memos->emplace(argtuple, f(args...));
        return memos->at(argtuple);
    });
}

// basically an integer that dereferences to itself
class SelfReferringInt {
    int i;

public:
    SelfReferringInt(int i): i(i) {}
    SelfReferringInt& operator++() {++i; return *this;}
    SelfReferringInt operator++(int) {
        SelfReferringInt old(*this);
        ++*this;
        return old;
    };
    bool operator==(SelfReferringInt& other) {return i == other.i;}
    bool operator==(int other) {return i == other;}
    bool operator!=(SelfReferringInt& other) {return i != other.i;}
    bool operator!=(int other) {return i != other;}
    int operator*() {return i;}

};
