#pragma once

#include <cmath>
#include <algorithm>
#include <functional>
#include <utility>
#include <map>
#include <memory>
#include <set>
#include <iostream>
#include <stdexcept>

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


//////////////////////////////////////////////////////////////////////
class DisengagedMaybeException : public std::runtime_error {
public:
    DisengagedMaybeException (const std::string& message) 
    : std::runtime_error(message) { };
};

template<typename T>
class Maybe
{
    T* objectptr;
    bool engaged;
public:
    Maybe() {engaged = false; objectptr = NULL;};
    Maybe(const Maybe<T>& other) {
        if (other.engaged) assign(other.value()); 
        else engaged = false; 
    };
    Maybe(T object) {
        assign(object);
    };
    Maybe(T* ptr) {
        if (ptr) assign(*ptr);
        else engaged = false;
    };
    template<typename ... Argtypes>
    Maybe(Argtypes ... args) {
        assign(T(args ...));
    };
    ~Maybe() {if (engaged) delete objectptr;};

    operator bool() const {return engaged;};
    Maybe<T>& operator= (const T& object) {
        if (engaged) delete objectptr; 
        assign(object);
        return *this;
    };
    Maybe<T>& operator= (const Maybe<T>& optionalobject) {
        if (engaged) delete objectptr; 
        engaged = optionalobject.engaged;
        if (engaged) objectptr = new T(optionalobject.value());
        return *this;
    };
    T* operator->() {
        if (!engaged) 
            throw DisengagedMaybeException("Attempted to access a nonexistent object");
        return objectptr;
    };
    T& operator*() {
        if (!engaged) 
            throw DisengagedMaybeException("Attempted to access a nonexistent object");
        return *objectptr;
    };
    const T& value() const {
        if (!engaged) 
            throw DisengagedMaybeException("Attempted to access a nonexistent object");
        return *objectptr;
    };
    template <typename U>
    T value_or(const U& value) const {
        if (engaged) return *objectptr;
        else return T(value);
    };
    
private:
    void assign(const T& obj) {
        engaged = true;
        objectptr = new T(obj);
    };
};


// returns all subsets of a given container, in no particular order
// the empty set is not included
template<
    template<typename ...> class CtnrType, 
    typename ValueType,
    typename... CtnrArgTypes >
auto getSubsets(const CtnrType<ValueType,CtnrArgTypes...>& ctnr)
    -> std::set<std::set<ValueType>>
{
    typedef std::set<std::set<ValueType>> ReturnType;
    typedef const CtnrType<ValueType,CtnrArgTypes...>& ParamType;
    std::function<ReturnType(ParamType,int)> recurrence = [&](ParamType c,int size) {
        if (size == 0)
            return ReturnType({std::set<ValueType>()});
        else {
            auto smallerSubsets = recurrence(c, size-1);
            ReturnType result;
            for (auto& s: smallerSubsets) {
                for (auto& v: c) {
                    std::set<ValueType> sCopy(s);
                    sCopy.insert(v);
                    result.insert(sCopy);
                }
            } 
            return result;
        }
    };
    return recurrence(ctnr, ctnr.size());
}
/*
template<
    template<typename ...> class CtnrType, 
    typename ValueType,
    typename... CtnrArgTypes >
class FixedSizeSubsetIterator
{
    int size;
    FixedSizeSubsetIterator<
}
*/
