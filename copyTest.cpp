#include "Value.h"
#include <iostream>
#include <string>
using namespace cppPyon;
using namespace std;

struct Tester {
    const char* _c;
    Tester(const char* c) { _c = c; cerr << "by c-str: "<< _c << endl; }
    Tester(const Tester& t) { this->_c = t._c; cerr << "copy:" << _c << endl; }
    //Tester(Tester&& t) { this->_c = t._c; cerr << "move:" << _c << endl; }
};

void foo(const Tester tester) {
    cout << "in foo:" << tester._c << endl;
}

int main(int argc,char** argv) {
    Tester tester("bar");
    foo(tester);
    foo("baz");
}
