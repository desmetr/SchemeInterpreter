#include "Environment.h"

Environment::Environment() 
    : dict(), outer(nullptr)
{}

Environment::Environment(const std::list<std::string>& symbols, 
                         const std::list<Expression>& values,
                         std::shared_ptr<Environment> outer) 
: outer(outer)
{
    static const auto makepair = [](const std::string& s, const Expression& v)
    {
        return std::pair<std::string,Expression>(s, v);
    };
    std::transform(symbols.cbegin(), symbols.cend(), values.cbegin(),
                   std::inserter(dict, dict.begin()), makepair);
}

Expression& Environment::find(const std::string& s) 
{
    auto candidate = dict.find(s);
    if (candidate != dict.end()) return candidate->second;
    else if (outer) return outer->find(s);
    else throw std::invalid_argument(s + " does not exist");
}

void Environment::addSymbol(const Symbol& s, const Expression& e){
    dict.insert(std::make_pair(s, e));
}

void Environment::setSymbol(const Symbol& s, const Expression& e){
    dict.at(s) = e;
}
