#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <tr1/memory>
using namespace std;
using namespace tr1;

namespace cppPyon {
    class Value;
    typedef shared_ptr< map<Value,Value> > MapPtr;
    typedef shared_ptr< string > StrPtr;
    typedef shared_ptr< vector<Value> > VecPtr;
    enum ValueType {Null,Bool,Int,Double,String,List,Mapping,Pyob};
    class Value {
        friend ostream& operator<<(ostream& output,const Value& value);
        private:
            MapPtr m_;
            StrPtr s_;
            VecPtr v_;
            int i_;
            double d_;
            ValueType vType_;
        public:
            Value() { vType_ = Null; };
            Value(bool b) { i_ = b; vType_ = Bool; };
            Value(int64_t i) {i_ = i; vType_ = Int;};
            Value(int i) {i_ = i; vType_ = Int;};
            Value(double d){d_ = d; vType_ = Double;};
            Value(const string& s){ 
                s_.reset( new string(s) );
                vType_ = String; 
            }; 
            Value(const char * p) { 
                s_.reset( new string(p) );
                vType_ = String; 
            };
            Value(ValueType vt) {
                if (vt == List) {
                    v_.reset( new vector<Value> );
                } else if (vt == Mapping || vt == Pyob) {
                    m_.reset( new map<Value,Value> );
                } else {
                    throw "bad constructor";
                }
                vType_ = vt;
            };
            void push_back(Value value) { v_->push_back(value); };
            string getRepr() const {
                if (vType_ == String) {
                    string out("'");
                    out += *s_;
                    out += "'";
                    return out;
                }
                if (vType_ == Null) return string("None");
                if (vType_ == Bool) {
                    if (i_) return string("True");
                    else return string("False"); }
                stringstream oss;
                if (vType_ == Int) oss << i_;
                if (vType_ == Double) oss << d_;
                if (vType_ == List) {
                    oss << "[";
                    vector<Value>::iterator v;
                    for (v=v_->begin();v!=v_->end();v++)
                        oss << (*v) << ",";
                    oss << "]";
                }
                if (vType_ == Mapping) {
                    map<Value,Value>::iterator m;
                    oss << "{";
                    for (m=m_->begin();m!=m_->end();m++)
                        oss << (*m).first << ":" << (*m).second << ",";
                    oss << "}";
                }
                if (vType_ == Pyob) throw "not implemented";
                return oss.str();
            };
            bool operator==(const Value & other) const {
                if (vType_ != other.vType_) return false;
                if (vType_ == String) 
                    return *s_ == *other.s_;
                if (vType_ == Null) return true;
                if (vType_ == Bool | vType_ == Int)
                    return i_ == other.i_;
                if ( vType_ == Double)
                    return d_ == other.d_;
                throw "eq test on compound types";
            };
            bool operator==(bool other) const {
                if (vType_ != Bool) return false;
                return other == (bool) i_;
            };
            bool operator==(int other) const {
                if (vType_ == Int) return other == (int) i_;
            };
            bool operator==(double other) const {
                if (vType_ == Double) return other == d_;
                if (vType_ == Int && other - (int) other == 0) 
                    return i_ == (int) other;
            };
            bool operator<(const Value & other) const {
                if (vType_ != other.vType_) return vType_ < other.vType_;
                if (vType_ == String) return *s_ < *other.s_;
                if (vType_ == Int || vType_ == Bool) return i_ < other.i_;
                if (vType_ == Null) return false;
                if (vType_ == Double) return d_ < other.d_;
                throw "comparison on compound types!";
            };
            Value& operator[](const Value v) {
                if (vType_ == Mapping || vType_ == Pyob) return (*m_)[v];
                throw "type not compatiable";
            };
            Value& operator[](const char * c) {
                if (vType_ == Mapping || vType_ == Pyob) 
                    return (*m_)[Value(c)];
                throw "type not compatiable";
            };
            Value& operator[](int n) {
                if (vType_ == Mapping || vType_ == Pyob) 
                    return (*m_)[Value(n)];
                if (vType_ == List)
                    return (*v_)[n];
                throw "type not compatiable";
            };
            void putName(const char * c){ putName(string(c)); };
            void putName(string s) { (*m_)[Value()] = s; };
            ValueType getType() const { return vType_; };
            Value& operator[](const string& str) {
                if (vType_ == Mapping || vType_ == Pyob) 
                    return (*m_)[Value(str)];
                throw "type not compatiable";
            };
    };
    inline 
    ostream& operator<<(ostream& output,const Value& value) {
        output << value.getRepr();
        return output;
    }
}
