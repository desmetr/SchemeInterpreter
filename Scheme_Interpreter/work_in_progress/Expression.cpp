#include "Expression.h"

//////////////////// ALLOCATION ////////////////////////////////////////////////
typedef std::function<void*(void)> Allocator;
static const auto allocateInt
    = Allocator([](){return (void*) new int();});
static const auto allocateDouble
    = Allocator([](){return (void*) new double();});

static const std::map<ExpressionType,Allocator> allocators =
    { {Int  , allocateInt   },
      {Float, allocateDouble} };

//////////////////// DEALLOCATION //////////////////////////////////////////////
typedef std::function<void(void*)> Deallocator;

static const auto deallocateInt
    = Deallocator([](void* ptr){delete (int*) ptr;});
static const auto deallocateDouble
    = Deallocator([](void* ptr){delete (double*) ptr;});

static const std::map<ExpressionType,Deallocator> deallocators =
    { {Int  , deallocateInt   },
      {Float, deallocateDouble} };


/////////////////// TRUTH TESTS ////////////////////////////////////////////////
typedef std::function<bool(void*)> TruthTest;

static const auto testInt = 
    TruthTest([](void* ptr){return *((int*) ptr) != 0;});
static const auto testDouble = 
    TruthTest([](void* ptr){return *((double*) ptr) != 0.f;});
static const auto testBoolean =
    TruthTest([](void* ptr){return (bool*) ptr;});
static const auto testSymbol =
    TruthTest([](void* ptr){return *((Symbol*) ptr) == "";});
static const auto testList =
    TruthTest([](void* ptr){
        return *((std::list<Expression>*) ptr) == 
            std::list<Expression>();
    });

static const std::map<ExpressionType,TruthTest> truthTests =
    { {Int      , testInt       },
      {Float    , testDouble    }, 
      {Boolean  , testBoolean   }, 
      {Sym      , testSymbol    }, 
      {List     , testList      }, 
      };

/////////////////// CLASS METHODS //////////////////////////////////////////////


Expression::Expression(): type(None) {
    expptr = nullptr;

}
Expression::Expression(int i): type(Int) {
    expptr = new int(i);
}

Expression::Expression(double d): type(Float) {
    expptr = new double(d);
}

Expression::Expression(const Expression& v): type(v.type) {
    expptr = allocators.at(type)();
}

Expression::~Expression() {
    if (expptr) deallocators.at(type)(expptr);
}

Expression::operator bool() const {
    if (expptr) return truthTests.at(type)(expptr);
    else return false;
}

const Symbol& Expression::getAsSymbol() const {
    return *((Symbol*) expptr);
}

const std::list<Expression>& Expression::getAsList() const {
    return *((std::list<Expression>*) expptr);
}

const Lambda& Expression::getAsLambda() const {
    return *((Lambda*) expptr);
}


/////////////////// LAMBDA ///////////////////////////////////////////////////// 

Lambda::Lambda(const Expression& exp, const std::list<Symbol>& paramSyms, 
               Environment* outer)
: exp(exp), paramSyms(paramSyms), outer(outer),
  nArgsRequired(paramSyms.size())
{}

Expression Lambda::operator() (const std::list<Expression>& params) {
    if (params.size() == nArgsRequired) {
        Environment newEnvironment(paramSyms, params, outer);
        return evaluate(exp, newEnvironment);
    }
    // else TODO
}
