cppPyon
=======

Example usage:

    #include "Parser.h"
    using namespace cppPyon;

    ...

    string foo = "[A(1,2,cheese='fries'),7.3,true,{'foo':Null}]";
    Value v = parse(foo);
    ASSERT(v.size() == 4);
    ASSERT(v[0].kind() == "A");
    ASSERT(v[0]["cheese"] == "fries");
    ASSERT(v[0][1] == 2);
    ASSERT(v[1] == 7.3);

