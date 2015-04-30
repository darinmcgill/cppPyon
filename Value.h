#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
using namespace std;

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
            double d_;
            int i_;
            ValueType vType_;
        public:

            ///////////////
            // accessors //
            ///////////////

            ValueType getType() const { return vType_; };

            MapPtr getMap() { 
                if (vType_ == Mapping || vType_ == Pyob) return m_;
                throw runtime_error("type not compatible");};

            VecPtr getVec() { 
                if (vType_ == List || vType_ == Pyob) return v_;
                throw runtime_error("type not compatible");};

            StrPtr getStr() {
                if (vType_ == String || vType_ == Pyob) return s_;
                throw runtime_error("type not compatible");
            }

            double getDouble() {
                if (vType_ == Double) return d_;
                if (vType_ == Int) return (double) i_;
                if (vType_ == Null) return 0.0/0.0;
                if (vType_ == Bool && i_) return 1.0;
                if (vType_ == Bool && ! i_) return 0.0;
                if (vType_ == String) return atof(s_->c_str());
                throw runtime_error("type not compatible"); 
            };

            int64_t getInt() {
                if (vType_ == Int) return i_;
                if (vType_ == Bool &&   i_) return 1;
                if (vType_ == Bool && ! i_) return 0;
                if (vType_ == Double) return (int64_t) d_;
                throw runtime_error("type not compatible"); 
            };

            bool getBool() {
                if (vType_ == Bool || vType_ == Int) return i_ != 0;
                if (vType_ == Double) return d_ != 0;
                if (vType_ == Null) return false;
                if (vType_ == Pyob) return true;
                if (vType_ == Mapping) return getSize() > 0;
                if (vType_ == List) return getSize() > 0;
                if (vType_ == String) return getSize() > 0; 
                throw runtime_error("bad type?");
            };

            int getSize() {
                if (vType_ == List) return v_->size();
                if (vType_ == Mapping) return m_->size();
                if (vType_ == Pyob) return v_->size();
                if (vType_ == String) return s_->size();
                throw runtime_error("type not compatible"); };

            const char* c_str() {
                if (vType_ == String) return s_->c_str();
                throw runtime_error("type not compatible");
            };
            
                
            //////////////////
            // CONSTRUCTORS //
            //////////////////
            Value() { vType_ = Null; };
            Value(bool b) { i_ = b; vType_ = Bool; };
            Value(int64_t i) {i_ = i; vType_ = Int;};
            Value(int i) {i_ = i; vType_ = Int;};
            Value(uint64_t i) {i_ = (int) i; vType_ = Int;};
            Value(double d){d_ = d; vType_ = Double;};
            Value(const string& s){ 
                s_.reset( new string(s) );
                vType_ = String; }; 
            Value(const char * p) { 
                s_.reset( new string(p) );
                vType_ = String; };
            Value(const char * p,size_t len) { 
                s_.reset( new string(p,len) );
                vType_ = String; };
            Value(ValueType vt) {
                if (vt == List || vt == Pyob) 
                    v_.reset( new vector<Value> );
                if (vt == Mapping || vt == Pyob) 
                    m_.reset( new map<Value,Value> );
                vType_ = vt; };

            //////////////
            // mutators //
            //////////////
            void putName(const char * c){ putName(string(c)); };
            void putName(string s) { 
                s_.reset( new string(s) ); 
                if (vType_ == Pyob) return;
                if (vType_ == List) {
                    m_.reset( new map<Value,Value> );
                    vType_ = Pyob;
                    return;
                }
                if (vType_ == Mapping)  {
                    v_.reset( new vector<Value> );
                    vType_ = Pyob;
                    return;
                }
                throw new runtime_error("can't putName");
            };
            void push_back(Value value) { v_->push_back(value); };
            void promote() {
                if (vType_ != String) 
                    throw runtime_error("can only promote strings");
                v_.reset( new vector<Value> );
                m_.reset( new map<Value,Value> );
                vType_ = Pyob;
            };
            void operator/=(Value value) {
                if (not ((vType_ == List) or (vType_ == Pyob)) ) 
                    throw runtime_error("operation not supported");
                v_->push_back(value);
            }


            ///////////////////
            // serialization //
            ///////////////////
            string getRepr() const {
                bool first = true;
                if (vType_ == String) {
                    string out("\"");
                    out += *s_;
                    out += "\"";
                    return out;
                }
                if (vType_ == Null) return string("null");
                if (vType_ == Bool) {
                    if (i_) return string("true");
                    else return string("false"); }
                stringstream oss;
                if (vType_ == Int) oss << i_;
                if (vType_ == Double) oss << d_;
                if (vType_ == List) {
                    oss << "[";
                    for (auto it=v_->begin();it !=v_->end();it++) {
                        if (not first) oss << ",";
                        oss << (*it);
                        first = false;
                    }
                    oss << "]";
                }
                if (vType_ == Mapping) {
                    oss << "{";
                    for (auto m=m_->begin();m!=m_->end();m++) {
                        if (not first) oss << ",";
                        oss << (*m).first << ":" << (*m).second ;
                        first = false;
                    }
                    oss << "}";
                }
                if (vType_ == Pyob) {
                    oss << *s_;
                    oss << "(";
                    for (int i=0;has_key(i);i++) {
                        if (not first) oss << ",";
                        oss << v_->at(i);
                        first = false;
                    }
                    for (auto m2 = m_->begin();m2 != m_->end(); m2++) {
                        if (not first) oss << ",";
                        if (m2->first.vType_ != String)
                            throw runtime_error("bad Pyob key");
                        oss << *(m2->first.s_) << "=" << m2->second;
                        first = false;
                    }
                    oss << ")";
                }
                return oss.str();
            };
            
            ////////////////////////
            // == and < operators //
            ////////////////////////
            bool operator==(const char * c) const {
                if (vType_ == String) return (*s_) == c;
                return false;
            }
            bool operator==(const Value & other) const {
                if (vType_ != other.vType_) return false;
                if (vType_ == String) 
                    return *s_ == *other.s_;
                if (vType_ == Null) return true;
                if (vType_ == Bool | vType_ == Int)
                    return i_ == other.i_;
                if ( vType_ == Double)
                    return d_ == other.d_;
                throw runtime_error("eq test on compound types");
            };
            bool operator==(bool other) const {
                if (vType_ != Bool) return false;
                return other == (bool) i_;
            };
            bool operator==(int other) const {
                if (vType_ == Int) return other == (int) i_;
                if (vType_ == Double && d_ - (int) d_ == 0)
                    return other == (int) d_;
                return false;
            };
            bool operator==(double other) const {
                if (vType_ == Double) return other == d_;
                if (vType_ == Int && other - (int) other == 0) 
                    return i_ == (int) other;
                return false;
            };
            bool operator<(const Value & other) const {
                if (vType_ != other.vType_) return vType_ < other.vType_;
                if (vType_ == String) return *s_ < *other.s_;
                if (vType_ == Int || vType_ == Bool) return i_ < other.i_;
                if (vType_ == Null) return false;
                if (vType_ == Double) return d_ < other.d_;
                throw runtime_error("comparison on compound types!");
            };

            ////////////////////
            // element access //
            ////////////////////
            Value& operator[](const string s) {
                if (vType_ == Mapping || vType_ == Pyob) return (*m_)[s];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](const char * c) {
                if (vType_ == Mapping || vType_ == Pyob) 
                    return (*m_)[Value(c)];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](int n) {
                if (vType_ == List || vType_ == Pyob) return (*v_)[n];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](Value v) {
                if (v.vType_ == Int && (vType_ == List || vType_ == Pyob))
                        return (*v_)[v.i_];
                if (vType_ == Mapping || vType_ == Pyob) return (*m_)[v];
                throw runtime_error("type not compatiable");
            };
            bool has_key(const char * c) const {
                if (! (vType_ == Mapping || vType_ == Pyob)) return false;
                return m_->count(string(c)) > 0; };
            bool has_key(int i) const {
                if (vType_ == List || vType_ == Pyob) return i < v_->size(); 
                if (vType_ == Mapping) return m_->count(Value(i)) > 0;
                return false;
            };
            bool has_key(Value v) const {
                if (v.vType_ == Int && (vType_ == List || vType_ == Pyob))
                        return v.i_ < v_->size(); 
                if (v.vType_ == Mapping || v.vType_ == Pyob)
                    return m_->count(v) > 0;
                return false;
            };
    };
    inline 
    ostream& operator<<(ostream& output,const Value& value) {
        output << value.getRepr();
        return output;
    }
}
