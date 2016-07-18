#pragma once
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <algorithm>
#include <set>
#include <limits>
using namespace std;

extern "C" {
int siphash(uint8_t *out, const uint8_t *in, uint64_t inlen, const uint8_t *k);
}

inline uint64_t myHash(const char* in, int inlen=-1) {
    uint64_t keys[2];
    keys[0] = 506097522914230528ULL;
    keys[1] = 1084818905618843912ULL;
    if (inlen == -1) inlen = strlen(in);
    uint64_t target;
    siphash((uint8_t* )&target,(uint8_t*) in, inlen, (uint8_t*) &(keys[0]));
    return target;
}

inline uint64_t myHash(uint64_t a, uint64_t b) {
    uint64_t ab[2];
    ab[0] = a;
    ab[1] = b;
    return myHash((const char*) &(ab[0]),16);
}

namespace cppPyon {
    class Value;
    typedef shared_ptr< map<Value,Value> > MapPtr;
    typedef shared_ptr< string > StrPtr;
    typedef shared_ptr< vector<Value> > VecPtr;
    enum ValueType {Null,Bool,Int,Double,String,List,Mapping,Pyob};
    class Value {
        friend Value operator<<(Value left, Value right);
        friend ostream& operator<<(ostream& output,const Value& value);
        friend Value operator<<(ValueType t,const Value& value);
        friend Value unionOf(Value left, Value rite);
        private:
            MapPtr m_;
            StrPtr s_;
            VecPtr v_;
            double d_ = std::numeric_limits<double>::quiet_NaN();
            int64_t i_;
            ValueType t_;
        public:

            ///////////////
            // accessors //
            ///////////////

            ValueType getType() const { return t_; };

            MapPtr getMap() { 
                if (t_ == Mapping or t_ == Pyob) return m_;
                throw runtime_error("getMap(): bad type:" + getRepr());};

            VecPtr getVec() { 
                if (t_ == List or t_ == Pyob) return v_;
                throw runtime_error("getVec(): bad type" + getRepr());};

            string getStr() {
                if (t_ == String or t_ == Pyob) return *s_;
                throw runtime_error("getStr(): bad type " + getRepr());
            }

            string kind() const {
                if (t_ != Pyob) 
                    throw runtime_error("not a pyob: " + getRepr());
                return *s_;
            }

            operator string() {
                if (t_ == String or t_ == Pyob) return *s_;
                throw runtime_error("string(): bad type " + getRepr());
            }

            operator double() {
                if (t_ == Double) return d_;
                if (t_ == Int) return (double) i_;
                if (t_ == Null) return 0.0/0.0;
                if (t_ == Bool && i_) return 1.0;
                if (t_ == Bool && ! i_) return 0.0;
                if (t_ == String) return atof(s_->c_str());
                throw runtime_error("double(): bad type" + getRepr()); 
            };

            operator size_t() {
                if (t_ == Bool && i_) return 1;
                if (t_ == Bool && ! i_) return 0;
                if (t_ == Int) {
                    if (i_ < 0) throw runtime_error("value is negative");
                    return static_cast<size_t>(i_);
                }
                if (t_ == Double) {
                    if (d_ < 0 or d_ != d_) 
                        throw runtime_error("value is invalid");
                    return static_cast<size_t>(d_);
                }
                throw runtime_error("can't be changed into a size_t");
            };

            operator bool() {
                if (t_ == Int or t_ == Bool) return (bool) i_;
                if (t_ == Double) return not (d_ == 0);
                throw runtime_error("bool(): bad type" + getRepr()); 
            }

            operator int() {
                if (t_ == Int or t_ == Bool) return i_;
                if (t_ == Double) return (int) d_;
                throw runtime_error("int(): bad type" + getRepr()); 
            }

            bool isNumber() const { return (t_ == Double or t_ == Int); }

            /*
            int getInt() {
                if (t_ == Int) return i_;
                if (t_ == Bool &&   i_) return 1;
                if (t_ == Bool && ! i_) return 0;
                if (t_ == Double) return (int) d_;
                throw runtime_error("type not compatible"); 
            };

            bool getBool() {
                if (t_ == Bool || t_ == Int) return i_ != 0;
                if (t_ == Double) return d_ != 0;
                if (t_ == Null) return false;
                if (t_ == Pyob) return true;
                if (t_ == Mapping) return size() > 0;
                if (t_ == List) return size() > 0;
                if (t_ == String) return size() > 0; 
                throw runtime_error("bad type?");
            };
            */

            size_t size() const {
                if (t_ == List) return v_->size();
                if (t_ == Mapping) return m_->size();
                if (t_ == Pyob) return v_->size();
                if (t_ == String) return s_->size();
                throw runtime_error("size(): bad type" + getRepr());
            };

            const char* c_str() {
                if (t_ == String) return s_->c_str();
                throw runtime_error("c_str(): bad type: " + getRepr());
            };
            
                
            //////////////////
            // CONSTRUCTORS //
            //////////////////
            Value() { t_ = Null; };
            Value(bool b) { i_ = b; t_ = Bool; };
            Value(int i) {i_ = i; t_ = Int;};
            Value(int64_t i) {i_ = i; t_ = Int;};
            Value(size_t s) {i_ = static_cast<int64_t>(s); t_ = Int;};
            Value(double d){d_ = d; t_ = Double;};
            Value(const string& s){ 
                s_.reset( new string(s) );
                t_ = String; }; 
            Value(const char * p) { 
                s_.reset( new string(p) );
                t_ = String; };
            Value(const char * p,size_t len) { 
                s_.reset( new string(p,len) );
                t_ = String; };
            Value(ValueType vt) {
                if (vt == List or vt == Pyob) 
                    v_.reset( new vector<Value> );
                if (vt == Mapping or vt == Pyob) 
                    m_.reset( new map<Value,Value> );
                t_ = vt; 
            };
            Value(pair<Value,Value> p) {
                m_.reset( new map<Value,Value> );
                (*m_)[p.first] = p.second;
            }


            //////////////
            // mutators //
            //////////////
            void putName(const char * c){ putName(string(c)); };
            void putName(string s) { 
                s_.reset( new string(s) ); 
                if (t_ == Pyob) return;
                if (t_ == List) {
                    m_.reset( new map<Value,Value> );
                    t_ = Pyob;
                    return;
                }
                if (t_ == Mapping)  {
                    v_.reset( new vector<Value> );
                    t_ = Pyob;
                    return;
                }
                throw new runtime_error("can't putName");
            };
            void push_back(Value value) { 
                if (not (t_ == Pyob or t_ == List)) 
                    throw runtime_error("can't push_back on this type");
                v_->push_back(value); 
            };

            template<typename... Args>
            void push_back(Value value,Args... args) {
                v_->push_back(value);
                this->push_back(args...);
            }

            void put(Value key, Value value) {
                if (not (t_ == Pyob or t_ == Mapping)) 
                    throw runtime_error("can't put on this type");
                (*m_)[key] = value;
            }

            template<typename... Args>
            void put(Value key, Value value, Args... args) {
                this->put(key,value);
                this->put(args...);
            }
            

            void promote() {
                if (t_ != String) 
                    throw runtime_error("can only promote strings");
                v_.reset( new vector<Value> );
                m_.reset( new map<Value,Value> );
                t_ = Pyob;
            };
            void operator<<=(Value value) {
                if (not ((t_ == List) or (t_ == Pyob)) ) 
                    throw runtime_error("operation not supported");
                v_->push_back(value);
            }

            void sort() {
                if (not (t_ == List))
                    throw runtime_error("can only sort lists");
                if (v_->size() < 2) return;
                std::sort(v_->begin(),v_->end());
            }

            ///////////////////
            // serialization //
            ///////////////////
            string getRepr() const {
                bool first = true;
                if (t_ == String) {
                    string out("'");
                    out += *s_;
                    out += "'";
                    return out;
                }
                if (t_ == Null) return string("null");
                if (t_ == Bool) {
                    if (i_) return string("true");
                    else return string("false"); }
                stringstream oss;
                if (t_ == Int) oss << i_;
                if (t_ == Double) {
                    if (round(d_) == d_)
                        oss << static_cast<int64_t>(d_);
                    else 
                        oss << to_string(d_);
                }
                if (t_ == List) {
                    oss << "[";
                    for (auto it=v_->begin();it !=v_->end();it++) {
                        if (not first) oss << ",";
                        oss << (*it);
                        first = false;
                    }
                    oss << "]";
                }
                if (t_ == Mapping) {
                    oss << "{";
                    for (auto m=m_->begin();m!=m_->end();m++) {
                        if (not first) oss << ",";
                        oss << (*m).first << ":" << (*m).second ;
                        first = false;
                    }
                    oss << "}";
                }
                if (t_ == Pyob) {
                    oss << *s_;
                    oss << "(";
                    for (int i=0;has_key(i);i++) {
                        if (not first) oss << ",";
                        oss << v_->at(i);
                        first = false;
                    }
                    for (auto m2 = m_->begin();m2 != m_->end(); m2++) {
                        if (not first) oss << ",";
                        if (m2->first.t_ != String)
                            throw runtime_error("bad Pyob key");
                        oss << *(m2->first.s_) << "=" << m2->second;
                        first = false;
                    }
                    oss << ")";
                }
                return oss.str();
            };
            
            ////////////////////////
            // comparison operators
            ////////////////////////
            bool operator==(const char * c) const {
                if (t_ == String) return (*s_) == c;
                return false;
            }
            bool operator!=(const Value& other) const {
                return not (*this == other);
            }
            bool operator==(const Value& other) const {
                if (t_ != other.t_) return false;
                if (t_ == String) 
                    return *s_ == *other.s_;
                if (t_ == Null) return true;
                if ((t_ == Bool) or (t_ == Int))
                    return i_ == other.i_;
                if ( t_ == Double)
                    return d_ == other.d_;
                if ( t_ == Pyob and *s_ != *other.s_) return false;
                if ( t_ == List or t_ == Pyob ) {
                    int n = v_->size();
                    if (n != other.v_->size()) return false;
                    for (int i=0;i<n;i++) 
                        if ((*this)[i] != other[i]) return false;
                } 
                if ( t_ == Mapping or t_ == Pyob ) {
                    int n = m_->size();
                    if (n != (other.m_)->size()) return false;
                    for (auto it = m_->begin(); it != m_->end(); ++it)
                        if (it->second != other[it->first]) return false;
                } 
                return true;
            };
            bool operator==(bool other) const {
                if (t_ != Bool) return false;
                return other == (bool) i_;
            };
            bool operator==(int other) const {
                if (t_ == Int) return other == (int) i_;
                if (t_ == Double && d_ - (int) d_ == 0)
                    return other == (int) d_;
                return false;
            };
            bool operator==(double other) const {
                if (t_ == Double) return other == d_;
                if (t_ == Int && other - (int) other == 0) 
                    return i_ == (int) other;
                return false;
            };
            bool operator<=(const Value& other) const {
                return not (other < *this);
            }
            bool operator>(const Value& other) const {
                return other < *this;
            }
            bool operator<(const Value& other) const {
                if (t_ != other.t_) return t_ < other.t_;
                if (t_ == String) return *s_ < *other.s_;
                if (t_ == Int || t_ == Bool) return i_ < other.i_;
                if (t_ == Null) return false;
                if (t_ == Double) return d_ < other.d_;
                if ( t_ == Pyob and *s_ != *other.s_) return *s_ < *other.s_;
                if ( t_ == List or t_ == Pyob ) {
                    int n = v_->size();
                    if (n != other.v_->size()) return n < other.v_->size();
                    for (int i=0;i<n;i++) 
                        if ((*v_)[i] != (*other.v_)[i]) 
                            return (*v_)[i] < (*other.v_)[i];
                } 
                if ( t_ == Mapping or t_ == Pyob ) {
                    int n = m_->size();
                    if (n != (other.m_)->size()) return n < (other.m_)->size();
                    for (auto it = m_->begin(); it != m_->end(); ++it)
                        if (it->second != (*other.m_)[it->first]) 
                            return (it->second < (*other.m_)[it->first]);
                } 
                return false;
            };

            ////////////////////
            // element access //
            ////////////////////
            Value& operator[](const string s) const {
                if (t_ == Mapping || t_ == Pyob) return (*m_)[s];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](const char * c) const {
                if (t_ == Mapping || t_ == Pyob) 
                    return (*m_)[Value(c)];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](int n) const {
                if (t_ == List || t_ == Pyob) return (*v_)[n];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](size_t n) const {
                if (t_ == List || t_ == Pyob) return (*v_)[n];
                throw runtime_error("type not compatiable");
            };
            Value& operator[](Value v) const {
                if (v.t_ == Int && (t_ == List || t_ == Pyob))
                        return (*v_)[v.i_];
                if (t_ == Mapping || t_ == Pyob) return (*m_)[v];
                throw runtime_error("type not compatiable");
            };
            Value get(Value key,Value ifMissing=Null) const {
                if (has_key(key)) return (*this)[key];
                return ifMissing;
            }
            bool has_key(const char * c) const {
                if (! (t_ == Mapping || t_ == Pyob)) return false;
                return m_->count(string(c)) > 0; };
            bool has_key(int i) const {
                if (t_ == List || t_ == Pyob) return i < v_->size(); 
                if (t_ == Mapping) return m_->count(Value(i)) > 0;
                return false;
            };
            bool has_key(Value v) const {
                if (v.t_ == Int && (t_ == List || t_ == Pyob))
                        return v.i_ < v_->size(); 
                if (v.t_ == Mapping || v.t_ == Pyob)
                    return m_->count(v) > 0;
                return false;
            };

            uint64_t hash() const {
                if (t_ == Null)
                    return 11924423634372684106ULL;
                if (t_ == Bool) {
                    if (i_) return 15048741373040539721ULL;
                    return 3842791678396607141ULL;
                }
                if (t_ == String)
                    return myHash(s_->c_str(),s_->size());
                if (t_ == Double) 
                    return myHash((const char*)&d_, 8);
                if (t_ == Int) 
                    return myHash((const char*)&i_, 4);
                if (t_ == List) {
                    uint64_t current = 17003034975299842994ULL;
                    for (Value& v : *v_) 
                        current = myHash(current,v.hash());
                    return current;
                }
                if (t_ == Mapping) {
                    uint64_t current = 1551069553966629517ULL;
                    for (auto& x : *m_) 
                        current = myHash(current,
                            myHash(x.first.hash(),x.second.hash()));
                    return current;
                }
                if (t_ == Pyob) {
                    uint64_t current = 3030619542563045960ULL;
                    current = myHash(current,myHash(s_->c_str(),s_->size()));
                    for (Value& v : *v_) 
                        current = myHash(current,v.hash());
                    for (auto& x : *m_) 
                        current = myHash(current,
                            myHash(x.first.hash(),x.second.hash()));
                    return current;
                }
                throw runtime_error("Value.hash: not implemented");
            }
    };
    inline 
    ostream& operator<<(ostream& output,const Value& value) {
        output << value.getRepr();
        return output;
    }
    inline 
    Value operator<<(ValueType t,const Value& value) {
        if (t != List) throw runtime_error("bad left for <<");
        Value out(List);
        out.v_->push_back(value);
        return out;
    }
    inline
    Value operator<<(Value left, Value right) {
        while (1) {
            if (left.t_ == List) break; 
            if (left.t_ == Pyob) break;
            throw runtime_error("bad <<");
        }
        left.v_->push_back(right);
        return left;
    }
    inline 
    Value operator/(ValueType left, string right) {
        if (left != Pyob) throw runtime_error("bad /");
        Value value = Pyob;
        value.putName(right);
        return value;
    }

    template<typename... Args>
    Value listOf(Args... args) // recursive variadic function
    {
        Value out = List;
        out.push_back(args...);
        return out;
    }

    using Values = initializer_list<Value>;
    inline
    Value makePyob(const char* kind, Values a=Values(), Values b=Values()) 
    {
        Value out = kind;
        out.promote();
        for (auto el : a) {
            out.push_back(el);
        }
        bool tick = true;
        Value key;
        for (auto el : b) {
            if (tick) {
                key = el;
                tick = false;
            } else {
                out[key] = el;
                tick = true;
            }
        }
        return out;
    }

    template<typename... Args>
    Value mapOf(Args... args) 
    {
        Value out = Mapping;
        out.put(args...);
        return out;
    }

    inline
    Value unique(Value aList) {
        if (aList.getType() != List) throw runtime_error("not a list");
        if (aList.size() < 2) return aList;
        set<Value> out;
        for (int i=0;i<aList.size();i++)
            out.insert(aList[i]);
        Value out2 = List;
        for (auto it = out.begin(); it != out.end(); it++)
            out2.push_back(*it);
        return out2;
    }

    inline
    Value unionOf(Value left, Value rite) {
        if (left.t_ != List) return unionOf(listOf(left),rite);
        if (rite.t_ != List) return unionOf(left,listOf(rite));
        Value out = List;
        left = unique(left);
        rite = unique(rite);
        auto leftIt = left.v_->begin();
        auto riteIt = rite.v_->begin();
        while (not (leftIt == left.v_->end() or riteIt == rite.v_->end())) {
            if (*leftIt == *riteIt) {
                out.push_back(*leftIt);
                leftIt++;
                riteIt++;
                continue;
            }
            if (*leftIt < *riteIt) {
                out.push_back(*leftIt);
                leftIt++;
                continue;
            }
            out.push_back(*riteIt);
            riteIt++;
        }
        while (leftIt != left.v_->end()) out.push_back(*(leftIt++));
        while (riteIt != rite.v_->end()) out.push_back(*(riteIt++));
        return out;
    }

    inline string operator+(const char* left, Value& rite) {
        string out = left;
        out += rite.getRepr();
        return out;
    }

    inline Value operator+(Value left, Value rite) {
        if (left.getType() == String && rite.getType() == String) {
            string con = left.getStr() + rite.getStr();
            return Value(con);
        }
        throw runtime_error("Value operator+ not implemented: " + 
            left.getRepr() + " " + rite.getRepr());
    }
}
