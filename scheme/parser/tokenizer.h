#pragma once

#include <array>
#include <iostream>
#include <istream>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <variant>

struct SymbolToken {
    std::string name;
    bool operator==(const SymbolToken& other) const;
};
struct QuoteToken {
    bool operator==(const QuoteToken&) const;
};
struct DotToken {
    bool operator==(const DotToken&) const;
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;
    bool operator==(const ConstantToken& other) const;
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in);
    bool IsEnd();
    Token GetToken();
    void Next();

private:
    std::istream* in_;
    bool f_ = false;
    Token t_;
    std::string s_;

    bool IsPassSymbol(char ch);
    bool IsDigit(char ch);
    bool IsSymbolOrBegin(char ch);
    bool IsBackSlash(char ch);
    bool IsSign(char ch);
    bool IsBracket(char ch);
    bool IsDot(char ch);
};