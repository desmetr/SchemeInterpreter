#include "Expression.h"

//////////////////// COPY CREATORS /////////////////////////////////////////////

typedef std::function<void*(void*)> CopyCreator;

template <typename T>
void* copyT(void* ptr) {
    T* copy = new T();
    *copy = *((T*) ptr);
    return (void*) copy;
}

void* lambdaCopyCreator(void*);
static const auto copyFunction
    = CopyCreator(&lambdaCopyCreator); 

static const std::map<ExpressionType,CopyCreator> copyCreators =
    { 
        {Int      , copyT<int>    },
        {Float    , copyT<double> },
        {Sym      , copyT<Symbol> },
        {List     , copyT<std::list<Expression>> },
        {Function , copyFunction  },
    };


//////////////////// DEALLOCATION //////////////////////////////////////////////
typedef std::function<void(void*)> Deallocator;
void lambdaDeallocator(void*);

template<typename T>
void deallocateT(void* ptr) {delete (T*) ptr;}

static const auto deallocateFunction
    = Deallocator(&lambdaDeallocator); // Lambda.h

static const std::map<ExpressionType,Deallocator> deallocators =
    { 
        {Int        , deallocateT<int>                      },
        {Float      , deallocateT<double>                   },
        {Sym        , deallocateT<Symbol>                   },
        {List       , deallocateT<std::list<Expression>>    },
        {Function   , deallocateFunction                    }
    };


/////////////////// TRUTH TESTS ////////////////////////////////////////////////
typedef std::function<bool(void*)> TruthTest;

static const auto testInt = 
    TruthTest([](void* ptr){return *((int*) ptr) != 0;});
static const auto testDouble = 
    TruthTest([](void* ptr){return *((double*) ptr) != 0.f;});
static const auto testBoolean =
    TruthTest([](void* ptr){return (bool*) ptr;});
static const auto testSymbol =
    TruthTest([](void* ptr){return *((Symbol*) ptr) != "";});
static const auto testList =
    TruthTest([](void* ptr){
        return *((std::list<Expression>*) ptr) == 
            std::list<Expression>();
    });

static const std::map<ExpressionType,TruthTest> truthTests =
    { 
        {Int      , testInt       },
        {Float    , testDouble    }, 
        {Boolean  , testBoolean   }, 
        {Sym      , testSymbol    }, 
        {List     , testList      }, 
    };

/////////////////// TYPE TO STRING MAP ///////////////////////////////////////// 

static const std::map<ExpressionType,std::string> typeToStringMap =
    {
        {Int      , "Int" },
        {Float    , "Float"}, 
        {Boolean  , "Boolean"}, 
        {Sym      , "Sym"}, 
        {List     , "List"}, 
        {Function , "Function"}, 
    };

/////////////////// CLASS METHODS //////////////////////////////////////////////

// CONSTRUCTORS
Expression::Expression(): type(None) {
    expptr = nullptr;
}

Expression::Expression(Symbol& s): type(Sym) {
    expptr = new Symbol(s);
}

Expression::Expression(int i): type(Int) {
    expptr = new int(i);
}

Expression::Expression(double d): type(Float) {
    expptr = new double(d);
}

Expression::Expression(const std::list<Expression>& l): type(List) {
    expptr = new std::list<Expression>(l);
}

Expression::Expression(Lambda& l): type(Function) {
    expptr = lambdaCopyCreator((void*) &l);
}

Expression::Expression(const Expression& v) {
    if (v.type > None) 
        throw std::runtime_error("attempt to copy-construct from nonexistant type");
    else type = v.type;
    expptr = copyCreators.at(v.type)(v.expptr);
}

Expression::~Expression() {
    if (expptr) deallocators.at(type)(expptr);
}

ExpressionType Expression::getType() const {
    return type;
}

int Expression::getAsInt() const {
    if (type == Int)
        return *((int*) expptr);
    else 
        throw std::runtime_error("getAsInt called on expression of type "); 
            //TODO
}

double Expression::getAsFloat() const {
    if (type == Float)
        return *((double*) expptr);
    else 
        throw std::runtime_error("getAsFloat called on expression of type "); 
            //TODO
}

const Symbol& Expression::getAsSymbol() const {
    if (type == Sym)
        return *((Symbol*) expptr);
    else 
        throw std::runtime_error("getAsSymbol called on expression of type " + typeToStringMap.at(type)); 
            //TODO
}

const Lambda& Expression::getAsFunction() const {
    if (type == Function)
        return *((Lambda*) expptr);
    else throw std::runtime_error("getAsFunction called on expression of type "); 
            //TODO
}
const std::list<Expression>& Expression::getAsList() const {
    if (type == List)
        return *((std::list<Expression>*) expptr);
    else 
        throw std::runtime_error("getAsList called on expression of type "); 
            //TODO
}

/////////////////// OPERATORS //////////////////////////////////////////////////

Expression::operator bool() const {
    if (expptr) return truthTests.at(type)(expptr);
    else return false;
}

Expression Expression::operator+(int i) const {
    if (type == Int)
        return Expression(getAsInt() + i);
    else
        return Expression(getAsFloat() + float(i));
            //TODO
}
Expression Expression::operator+(double d) const {
    if (type == Int)
        return Expression(float(getAsInt()) + d);
    else 
        return Expression(getAsFloat() + d);
            //TODO
}
Expression Expression::operator+(const Expression& e) const {
    if (type == Int)
        return e + getAsInt();
    else if (type == Float)
        return e + getAsFloat();
    else
        throw std::runtime_error("operator+ called on expression of type ");
            //TODO
}

Expression Expression::operator-(int i) const {
    if (type == Int)
        return Expression(getAsInt() - i);
    else
        return Expression(getAsFloat() - float(i));
            //TODO
}
Expression Expression::operator-(double d) const {
    if (type == Int)
        return Expression(float(getAsInt()) - d);
    else 
        return Expression(getAsFloat() - d);
            //TODO
}
Expression Expression::operator-(const Expression& e) const {
    if (type == Int)
        return e - getAsInt();
    else if (type == Float)
        return e - getAsFloat();
    else
        throw std::runtime_error("operator+ called on expression of type ");
            //TODO
}

Expression Expression::operator*(int i) const {
    if (type == Int)
        return Expression(getAsInt() * i);
    else
        return Expression(getAsFloat() * float(i));
            //TODO
}
Expression Expression::operator*(double d) const {
    if (type == Int)
        return Expression(float(getAsInt()) * d);
    else 
        return Expression(getAsFloat() * d);
            //TODO
}
Expression Expression::operator*(const Expression& e) const {
    if (type == Int)
        return e * getAsInt();
    else if (type == Float)
        return e * getAsFloat();
    else
        throw std::runtime_error("operator+ called on expression of type ");
            //TODO
}

Expression Expression::operator/(int i) const {
    if (type == Int)
        return Expression(getAsInt() / i);
    else
        return Expression(getAsFloat() / float(i));
            //TODO
}
Expression Expression::operator/(double d) const {
    if (type == Int)
        return Expression(float(getAsInt()) / d);
    else 
        return Expression(getAsFloat() / d);
            //TODO
}
Expression Expression::operator/(const Expression& e) const {
    if (type == Int)
        return e / getAsInt();
    else if (type == Float)
        return e / getAsFloat();
    else
        throw std::runtime_error("operator+ called on expression of type ");
            //TODO
}

/////////////////// LAMBDA MEMORY STUFF ////////////////////////////////////////
#include "Lambda.h"

void* lambdaCopyCreator(void* ptr) {
    return copyT<Lambda>(ptr);
}

void lambdaDeallocator(void* ptr) {
    delete (Lambda*) ptr;
}
