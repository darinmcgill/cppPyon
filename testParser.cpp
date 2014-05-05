#include "Parser.h"
#include <sys/time.h>
using namespace cppPyon;

double tv2double(timeval tv) {
    double out = tv.tv_usec / 1e6;
    out += tv.tv_sec;
    return out;
};

double getCurrentTime() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv2double(tv);
};

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

    Parser *p = new Parser();
    double start = getCurrentTime();
    Value v = p->parse("Ab([92],foo={3:12},bar=None)");
    double end = getCurrentTime();
    cout << "parsed:" << v << "\nin " << 1e6*(end - start) << "us" << endl;
    } 
    catch (const char * c) {
        cout << "exception: " << c << endl;
    }
    catch (string s) {
        cout << "exception: " << s << endl;
    }
}
