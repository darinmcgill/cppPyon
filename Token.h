#pragma once
#include "Value.h"

namespace cppPyon {

    enum TokenType {
        End = 0,
        Comma = ',',
        OpenParen = '(',
        CloseParen = ')',
        OpenBracket = '[',
        CloseBracket = ']',
        OpenCurly = '{',
        CloseCurly = '}',
        Eq = '=', 
        Colon = ':',
        Quoted = 1,
        Bareword = 2,
        Number = 3
    };
    
    class Token {
        friend ostream& operator<<(ostream& output,const Token& token);
        private:
            TokenType type_;
            Value value_;
            void readString(char ** readFrom) {
                char quote = **readFrom;
                (*readFrom)++;
                char * start = (*readFrom);
                while (**readFrom != quote && **readFrom != 0) 
                    (*readFrom)++;
                value_ = Value(start,(*readFrom) - start);
                (*readFrom)++;
                type_ = Quoted;
            };
            bool isWhiteSpace(char w) {
                return w == ' ' || w == '\t' || w == '\n';
            };
            bool isDigit(char d) {
                return (d >= '0' && d <= '9');
            };
            void readNumber(char ** readFrom) {
                type_ = Number;
                int sign = 1;
                if ((**readFrom) == '-') {
                    sign = -1;
                    (*readFrom)++; }
                if ((**readFrom) == '+') (*readFrom)++;
                bool floating = false;
                int beforeDecimal = 0;
                double afterDecimal = 0.0;
                int e = 0;
                bool eneg = false;
                double multiplier = 0.1;
                while (isDigit(**readFrom)) {
                    beforeDecimal *= 10;
                    beforeDecimal += (**readFrom) - '0';   
                    (*readFrom)++; }
                if ((**readFrom) == '.') {
                    (*readFrom)++;
                    floating = true;
                    double multiplier = 0.1;
                    double tmp;
                    while (isDigit(**readFrom)) {
                        afterDecimal += multiplier * ( (**readFrom) - '0' );
                        (*readFrom)++;
                        multiplier *= 0.1; } }
                if ((**readFrom) == 'e' || (**readFrom) == 'E') {
                   (*readFrom)++;
                    floating = true;
                    if ((**readFrom) == '+') (*readFrom)++;
                    if ((**readFrom) == '-') { (*readFrom)++; eneg = true; }
                    while (isDigit(**readFrom)) {
                        e = e * 10;
                        e += (**readFrom) - '0';
                        (*readFrom)++; }
                }
                if (!floating) {
                    value_ = Value(sign * beforeDecimal);
                    return; }
                double out = sign*(beforeDecimal + afterDecimal);
                if (e) {
                    int i;
                    if (eneg) for (i=0;i<e;i++) out *= 0.1;
                    else for (i=0;i<e;i++) out *= 10;
                }
                value_ = Value(out);
            };
            bool wordChar(char c) {
                if (c >= 'a' && c <= 'z') return true;
                if (c >= 'A' && c <= 'Z') return true;
                if (c >= '0' && c <= '9') return true;
                if (c == '_') return true;
                return false;
            };
            void readBareword(char ** readFrom) {
                char * start = *readFrom;
                while (wordChar(**readFrom)) (*readFrom)++;
                value_ = Value(start,(*readFrom) - start);
                type_ = Bareword;
            };
        public:
            Token(TokenType t) { type_ = t; };
            Token(TokenType t,Value v) {
                type_ = t;
                value_ = v;
            };
            Token(char **readFrom) {
                while (isWhiteSpace(**readFrom)) (*readFrom)++;
                char first = **readFrom;
                switch (first) {
                    case 0: type_ = End; return;
                    case ',': 
                    case '(':
                    case ')':
                    case '[':
                    case ']':
                    case '{':
                    case '}':
                    case '=':
                    case ':':
                        type_ = (TokenType) first; 
                        (*readFrom) += 1;
                        return;
                    case '\'': 
                    case '"': 
                        readString(readFrom);
                        return;
                    case '+':
                    case '-':
                    case '.':
                        readNumber(readFrom);
                        return;
                    case '_':
                        readBareword(readFrom);
                        return;
                };
                if (first >= '0' && first <= '9') readNumber(readFrom);
                else if (first >= 'a' && first <= 'z') readBareword(readFrom);
                else if (first >= 'A' && first <= 'Z') readBareword(readFrom);
                else throw "don't know how to read";
            };
            string getRepr() const {
                if (type_ > 10) 
                    return "Syntax('" + 
                            string(1,(char)type_) + "')";
                if (type_ == Bareword)
                    return "Bareword(" + value_.getRepr() + ")";
                if (type_ == Quoted)
                    return "Quoted(" + value_.getRepr() + ")";
                if (type_ == Number)
                    return "Number(" + value_.getRepr() + ")";
                if (type_ == End) return "End()";
            };
            TokenType getType() { return type_; };
            Value getValue() { return value_; };
            bool operator==(char c) const {
                return c == (char) type_;
            };
    };
    inline 
    ostream& operator<<(ostream& output,const Token& token) {
        output << token.getRepr();
        return output;
    }
}
