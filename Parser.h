#pragma once
#include "Token.h"
#include <deque>

using namespace std;
namespace cppPyon {
    class Parser {
        private:
            deque<Token> tokens_;
            Token pop() {
                if (tokens_.empty()) 
                    throw runtime_error("ran out of tokens?");
                Token out = tokens_[0];
                tokens_.pop_front();
                return out;
            };
            //stack<Value> stack_;
            Value readPyob(Token bareWord) {
                Value out = bareWord.getValue();
                out.promote();
                while (1) {
                    if (tokens_.empty()) 
                        throw runtime_error("ran out of tokens parsing Pyob");
                    if (tokens_[0] == ')') {
                        tokens_.pop_front();
                        return out; }
                    if (tokens_[0] == ',') {
                        tokens_.pop_front();
                        continue; }
                    if (tokens_[0] == '}' || tokens_[0] == ']')
                        throw runtime_error("mismatched (");
                    if (tokens_[0].getType() == Bareword) {
                        if (tokens_.size() > 3 && tokens_[1] == '=') {
                            string key(tokens_[0].getValue().c_str());
                            tokens_.pop_front();
                            tokens_.pop_front();
                            out[key] = readValue();
                            continue; } }
                    out.push_back( readValue() );
                }
            };
            Value readList() {
                Value out(List);
                while (1) {
                    if (tokens_.empty()) 
                        throw runtime_error("ran out of tokens parsing List");
                    if (tokens_[0] == ']') {
                        tokens_.pop_front();
                        return out; }
                    if (tokens_[0] == ',') {
                        tokens_.pop_front();
                        continue; }
                    if (tokens_[0] == ')' || tokens_[0] == '}')
                        throw runtime_error("mismatched [");
                    out.push_back( readValue() );
                }
            };
            Value readMapping() {
                Value out(Mapping);
                while (1) {
                    if (tokens_.empty()) 
                        throw runtime_error("ran out of tokens for Mapping");
                    if (tokens_[0] == '}') {
                        tokens_.pop_front();
                        return out; }
                    if (tokens_[0] == ',') {
                        tokens_.pop_front();
                        continue; }
                    if (tokens_.size() >= 4 && tokens_[1] == ':') {
                        Value key = readValue();
                        tokens_.pop_front();
                        out[key] = readValue();
                        continue;
                    }
                    cerr << "while parsing mapping:" << tokens_[0] << endl;
                }
            };
            Value readValue() {
                if (tokens_.empty()) throw runtime_error("out of tokens?");
                Token t = tokens_.at(0);
                tokens_.pop_front();
                if (t.getType() == Number) return t.getValue();
                if (t.getType() == Quoted) return t.getValue();
                if (t.getType() == Bareword) {
                    if ((!tokens_.empty()) && tokens_.front() == '(') {
                        tokens_.pop_front();
                        return readPyob(t);
                    }
                    Value v = t.getValue();
                    if (v == "true" || v == "True") return Value(true);
                    if (v == "false" || v == "False") return Value(false);
                    if (v == "null" || v == "None") return Value();
                    throw string("parse error at:") + v.c_str();
                }
                if (t == '[') return readList();
                if (t == '{') return readMapping();
                cerr << "don't know what to do with" << t.getRepr() << endl;
                throw runtime_error("incomplete FDKJDF"); }
        public:
            Value parse(const char * input) {
                char* current = (char *) input;
                while (*current != 0) tokens_.push_back(Token(&current));
                return readValue();
            };
    };
}
