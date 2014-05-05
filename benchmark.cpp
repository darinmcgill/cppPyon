#include "Parser.h"
#include <sys/time.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>

using namespace std;
using namespace cppPyon;

double tv2double(timeval tv) {
    double out = tv.tv_usec / 1e6;
    out += tv.tv_sec;
    return out;
}
double getCurrentTime() {
    timeval tv;
    gettimeofday(&tv, NULL);
    return tv2double(tv);
};

int main(int argc,char* argv[]){
    if (argc < 2) {
        cerr << "please specify a filename" << endl;
        return 1; }
   
    ifstream t(argv[1]);
    string str((istreambuf_iterator<char>(t)),
                     istreambuf_iterator<char>());
    const char * parseMe = str.c_str();
    double p1 = getCurrentTime();
    Value out = Parser().parse(parseMe);
    double p2 = getCurrentTime();
    string back = out.getRepr();
    double p3 = getCurrentTime();
    cout << "parse:" << 1e6*(p2 - p1) << endl;
    cout << "dump:" << 1e6*(p3 - p2) << endl;
    cerr << back << endl;
}
