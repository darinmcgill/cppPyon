rm -f *.out
g++ -std=c++11 testValue.cpp siphash24.o -o testValue.out
g++ -std=c++11 testToken.cpp -o testToken.out
g++ -std=c++11 testParser.cpp -o testParser.out
g++ -std=c++11 benchmark.cpp -o benchmark.out
