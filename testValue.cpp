#include "Value.h"
#include <iostream>
#include <string>
using namespace cppPyon;
using namespace std;

int main(int argc,char* argv[]) {

    Value v = "abc";
    cout << v.hash() << endl;
    cout << Value(Null).hash() << endl;
    cout << Value(true).hash() << endl;
    cout << Value(false).hash() << endl;
    cout << Value(List).hash() << endl;
    cout << listOf(1).hash() << endl;
    Value mm = Mapping;
    mm["foo"] = "bar";
    cout << mm.hash() << endl;
    Value abcd = makePyob("foo",{"bar",3});
    cout << abcd.hash() << endl;
    return 0;


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

    Value p5 = "foo";
    cout << p5.getType() << endl;

    /*
    Value p6 = List << "hello" << "world" << 32;
    p6 << Null << false;
    cout << p6 << endl;
    */
    //p6.putName("bar");
    //cout << p6 << endl;

    //Value p7 = List << "bar" << 7;
    //p7 << "foo";
    //cout << p7 << endl;

    Value p7 = Pyob / "foobar";
    p7 <<= "cheese";
    cout << p7 << endl;

    Value p8 = Mapping;
    cout << p8 << endl;

    Value p90 = List;
    Value p91 = List;
    p90.push_back(5);
    p90.push_back(7);
    p91.push_back(5);
    p91.push_back(7);
    cout << "comps:" << endl;
    cout << (p91 == p90) << endl;
    cout << (p90 < p91) << endl;
    cout << (p90 > p91) << endl;
    cout << "---" << endl;
    p91[1] = 9;
    cout << (p90 == p91) << endl;
    cout << (p90 < p91) << endl;
    cout << (p90 > p91) << endl;
    cout << p91 << endl;

    Value q0 = List;
    q0.push_back(3,"hello",Null);
    cout << q0 << endl;

    Value q1 = listOf(7.5,Null,true,"tacos",List);
    cout << q1 << endl;

    Value q3 = mapOf("cheese",23,"fries",73);
    cout << q3 << endl;

    Value q4 = listOf(7,3,4,"cheese",true,"fries");
    q4.sort();
    cout << q4 << endl;

    Value j1 = listOf(2,3,5,3,7,"foo",true,Null,5,Null);
    Value j2 = unique(j1);
    cout << "unique: " << j2 << endl;

    Value q51 = listOf(7,1,4,9);
    Value q52 = listOf(7,13,7,1);
    Value q53 = unionOf(q51,q52);
    cout << q53 << endl;
    Value q54 = unionOf(q53,8);
    cout << q54 << endl;


    Value v1 = makePyob("cheese",{23,true,"fries"});
    cout << v1 << endl;

    Value v2 = makePyob("Thumps",{10});
    cout << (double) v2[0] << endl;


}
