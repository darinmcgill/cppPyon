#include <iostream>
#include "memory.h"

extern "C" {
int siphash(uint8_t *out, const uint8_t *in, uint64_t inlen, const uint8_t *k);
}

using namespace std;
int main(int argc,char** argv) {
    uint8_t key[16];
    for (uint8_t i=0;i<16;i++) { key[i] = i; }
    uint64_t target;
    siphash((uint8_t*) &target, (uint8_t *)argv[1],strlen(argv[1]),key);
    cout << target << endl;
    auto p = (uint64_t*)&(key[0]);
    cout << p[0] << endl;
    cout << p[1] << endl;
}


