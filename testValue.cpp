#include "Value.h"
#include <iostream>
#include <string>
using namespace cppPyon;
using namespace std;

int main(int argc,char* argv[]) {
    cout << "ValueType:" << sizeof(ValueType) << endl;
    cout << "Value:" << sizeof(Value) << endl;

    cout << "True:" << Value(true) << endl;
    cout << "True:" << Value(false) << endl;
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
    m[Value()] = Value(List);
    m["foo"] = "bar";
    m[3] = 19;
    m["other"] = l;
    cout << m << endl;
    l.push_back(99);
    cout << m << endl;
}
