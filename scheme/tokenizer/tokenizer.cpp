#include <error.h>
#include <tokenizer.h>

#include <array>
#include <iostream>
#include <istream>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>

bool SymbolToken::operator==(const SymbolToken& other) const {
    return name == other.name;
}

bool QuoteToken::operator==(const QuoteToken&) const {
    return true;  //?
}

bool DotToken::operator==(const DotToken&) const {
    return true;  //?
}

bool ConstantToken::operator==(const ConstantToken& other) const {
    return value == other.value;
}

Tokenizer::Tokenizer(std::istream* in) {
    in_ = in;
    Next();
}

bool Tokenizer::IsEnd() {
    return f_;
}

Token Tokenizer::GetToken() {
    return t_;
}

void Tokenizer::Next() {
    if (in_->peek() == EOF) {
        f_ = true;
    } else {
        while (in_->peek() != EOF) {
            if (IsPassSymbol(in_->peek())) {
                continue;
            }
            if (IsDigit(in_->peek()) || IsSymbolOrBegin(in_->peek()) || IsSign(in_->peek()) ||
                IsBackSlash(in_->peek()) || IsBracket(in_->peek()) || IsDot(in_->peek())) {
                break;
            }
            throw SyntaxError("");
        }
        s_ = "";
    }
}

bool Tokenizer::IsPassSymbol(char ch) {
    if (ch == '\n' || ch == ' ') {
        in_->get();
        if (in_->peek() == EOF) {
            f_ = true;
        }
        return true;
    }
    return false;
}

bool Tokenizer::IsBackSlash(char ch) {
    if (ch == '\'') {
        t_.emplace<3>();
        in_->get();
        return true;
    }
    return false;
}

bool Tokenizer::IsBracket(char ch) {
    if (ch == '(' || ch == ')') {
        BracketToken p = ch == '(' ? BracketToken::OPEN : BracketToken::CLOSE;
        t_.emplace<1>(p);
        in_->get();
        return 1;
    }
    return 0;
}

bool Tokenizer::IsDot(char ch) {
    if (ch == '.') {
        t_.emplace<4>();
        in_->get();
        return 1;
    }
    return 0;
}

bool Tokenizer::IsDigit(char ch) {
    if (std::isdigit(ch)) {
        while (std::isdigit(ch)) {
            s_ += in_->get();
            ch = in_->peek();
        }
        ConstantToken c;
        c.value = std::atoi(s_.begin().base());
        t_.emplace<0>(c);
        return 1;
    }
    return 0;
}

bool Tokenizer::IsSymbolOrBegin(char ch) {
    if (std::isalpha(ch) || (ch == '<') || (ch == '=') || (ch == '>') || (ch == '*') ||
        (ch == '/') || (ch == '#')) {
        while (std::isalpha(ch) || std::isdigit(ch) || (ch == '?') || (ch == '!') || (ch == '-') ||
               (ch == '<') || (ch == '=') || (ch == '>') || (ch == '*') || (ch == '/') ||
               (ch == '#')) {
            s_ += in_->get();
            ch = in_->peek();
        }
        SymbolToken c;
        c.name = s_;
        t_.emplace<2>(c);
        return 1;
    }
    return 0;
}

bool Tokenizer::IsSign(char ch) {
    if (ch == '-' || ch == '+') {
        s_ += ch;
        in_->get();
        if (!std::isdigit(in_->peek())) {
            SymbolToken c;
            c.name = s_;
            t_.emplace<2>(c);
        } else {
            IsDigit(in_->peek());
        }
        return 1;
    }
    return 0;
}