#include "Token.h"
using namespace cppPyon;
using namespace std;

int main(int argc,char* argv[]) {
    char input[] = "( )[ ]'foo '\"bar\" cheese 3 3.1 0.2 .3 +.3 -17 -0.33 7e2 -7.3e-1";
    char *current = input;
    while (*current != 0) {
        Token t(& current);
        cout << t.getRepr() << endl;
    }
    cout << Token(Comma).getRepr() << endl;
    cout << Token(OpenParen).getRepr() << endl;
    cout << Token(Bareword,Value("foo")).getRepr() << endl;
    cout << Token(Quoted,Value("foo")).getRepr() << endl;
}
