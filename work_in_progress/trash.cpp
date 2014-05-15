#include <functional>
#include <list>
using namespace std;

class C {

public:
    int i;

    C(int i): i(i) {}
};

C modC(C c, int i, char* ptr) {return C(i);}

typedef function<C(list<C>&)> LambdaType;
class Lambda 
{
    LambdaType lambda;
    int nArgsRequired;

public:
    Lambda(const C& c, int& ref, char* ptr) {
        lambda = LambdaType([=](list<C>& clist) -> C {
            char newChar = *ptr;
            return modC(c, ref, ptr);
        });
    }
};

int main() {
    C c(3);
    int i = 4;
    char d = 'c';
    Lambda(c, i, &d);

}
