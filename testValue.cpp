#include "Value.h"
#include <iostream>
#include <string>
using namespace cppPyon;
using namespace std;

int main(int argc,char* argv[]) {
    cout << "sizeof ValueType:" << sizeof(ValueType) << endl;
    cout << "sizeof Value:" << sizeof(Value) << endl;
    cout << "sizeof string:" << sizeof(string) << endl;

    cout << "True:" << Value(true) << endl;
    cout << "False:" << Value(false) << endl;
    cout << "None:" << Value() << endl;
    cout << "string:" << Value("string") << endl;
    cout << "------------" << endl;

    Value l = Value(List);
    l.push_back(Value());
    l.push_back(Value(3));
    l.push_back(Value("foo"));
    l.push_back(Value(List));
    cout << "list:" <<  l << endl;
    cout << "want=>" << (Value(true) == true) << endl;
    cout << "not =>" << (Value(true) == false) << endl;
    cout << "want=>" << (Value(17) == 17) << endl;
    cout << "not =>" << (Value(17) == 19) << endl;
    cout << "want=>" << (Value(17.0) == 17) << endl;
    Value m = Value(Mapping);
    m["foo"] = "bar";
    m["3"] = 19;
    m["other"] = l;
    cout << m << endl;
    l.push_back(99);
    cout << m << endl;
    
    Value p(Pyob);
    p.putName("A");
    cout << p << endl;

    Value p2(Pyob);
    p2.putName("A");
    p2.push_back(Value(7));
    cout << p2 << endl;

    Value p3(Pyob);
    p3.putName("A");
    p3["b"] = l;
    cout << p3 << endl;

    Value p4(Pyob);
    p4.putName("A");
    p4["b"] = "abc";
    p4.push_back(17);
    cout << p4 << endl;
}
