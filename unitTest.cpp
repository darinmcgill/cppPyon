#include "Parser.h"
using namespace cppPyon;


using Tests = map<string,function<void(bool)>>;

bool runTests(int argc, char** argv, Tests& tests) {
    if (argc == 2) {
        tests[argv[1]](true);
        return 0;
    }

    cerr << endl;
    int failures = false;
    for (auto it=tests.begin();it != tests.end(); it++) {
        cerr << it->first << "";
        try {
            it->second(false);
            cerr << "" << endl;
        } catch (runtime_error e) {
            failures = true;
            cerr << "......................failed" << endl;
        }
    }
    cerr << endl;
    return failures;
}

void assertTrue(bool b,const char* file, int line) { 
    if (!b) { 
        string msg = "assertion failed in "; 
        msg += file; 
        msg += " at " + to_string(line); 
        throw runtime_error(msg); 
    }
}

#define ASSERT(x) assertTrue(x,__FILE__,__LINE__)


int main(int argc, char** argv) {
    Tests tests;    
    
    tests["ok"] = [](bool verbose) {
        ASSERT(true);
    };  

    tests["problem"] = [](bool verbose) {
        //string s = "['00:00:00.048691093',1439856000048.691162,4181.000000,4183.000000,3.000000,2.000000,2.000000,7.000000,nan,0.000000,nan,nan,nan,nan,4182000000.000000,nan]";
        string s = "1439856000048.691162";
        Value v = parse(s);
        if (verbose) cerr << v << endl;
        ASSERT(static_cast<double>(v) > 1439856000048.);
        ASSERT(static_cast<double>(v) < 1439856000049.);
    };

    tests["display"] = [](bool verbose) {
        string s = "1439856000048.691162";
        Value v = parse(s);
        ASSERT(s == v.getRepr());

        s = "3.0";
        v = parse(s);
        ASSERT(v.getRepr() == "3");
    };

    tests["commentLine"] = [] (bool verbose) {
        string foo = "[3,2,# hello world 5\n7, // nevermore 8\n 13]";
        Value v = parse(foo);
        ASSERT(v.size() == 4);
        ASSERT(v[0] == 3);
        ASSERT(v[1] == 2);
        ASSERT(v[2] == 7);
        ASSERT(v[3] == 13);
    };

    tests["commentBlock"] = [] (bool verbose) {
        string foo = "[3,2,/* hello world 5\n7, // nevermore 8 */\n 13]";
        Value v = parse(foo);
        if (verbose) cerr << v << endl;
        ASSERT(v.size() == 3);
        ASSERT(v[0] == 3);
        ASSERT(v[1] == 2);
        ASSERT(v[2] == 13);
    };


    runTests(argc,argv,tests);

}
