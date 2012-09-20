#include "Parser.h"
using namespace cppPyon;

int main(int argc,char* argv[]) {
    try {
    cout << Parser().parse("3.2") << endl;
    cout << Parser().parse("'cheese'") << endl;
    cout << Parser().parse("true") << endl;
    cout << Parser().parse("null") << endl;
    cout << Parser().parse("blue()") << endl;
    cout << Parser().parse("blue(3)") << endl;
    cout << Parser().parse("blue(3,19)") << endl;
    cout << Parser().parse("blue(x=9)") << endl;
    cout << Parser().parse("red(17,3,y=9,x=99)") << endl;
    cout << Parser().parse("[17,true]") << endl;
    cout << Parser().parse("{'red':13,'blue':'foo'}") << endl;
    cout << Parser().parse("{13:'red','blue':[true,None]}") << endl;
    } 
    catch (const char * c) {
        cout << "exception: " << c << endl;
    }
    catch (string s) {
        cout << "exception: " << s << endl;
    }
}
