#include <error.h>
#include <parser.h>

#include <array>
#include <iostream>
#include <istream>
#include <memory>
#include <optional>
#include <random>
#include <stdexcept>
#include <string>
#include <variant>

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("");
    }
    if (tokenizer->GetToken().index() == 0) {
        ConstantToken token = std::get<ConstantToken>(tokenizer->GetToken());
        tokenizer->Next();
        Number n;
        n.Set(token.value);
        return std::make_shared<Number>(n);
    }
    if (tokenizer->GetToken().index() == 1) {
        if (tokenizer->GetToken() == Token{BracketToken::OPEN}) {
            return ReadList(tokenizer);
        }
        throw SyntaxError("");
    }
    if (tokenizer->GetToken().index() == 2) {
        SymbolToken token = std::get<SymbolToken>(tokenizer->GetToken());
        tokenizer->Next();
        Symbol n;
        n.Set(token.name);
        return std::make_shared<Symbol>(n);
    }
    if (tokenizer->GetToken().index() == 4) {
        tokenizer->Next();
        Symbol n;
        std::string dot = ".";
        n.Set(dot);
        return std::make_shared<Symbol>(n);
    }
    throw SyntaxError("");
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    size_t size = 0;
    tokenizer->Next();
    Token token;
    if (!tokenizer->IsEnd()) {
        token = tokenizer->GetToken();
    }
    std::shared_ptr<Cell> ans = std::make_shared<Cell>();
    std::shared_ptr<Cell> ptr = ans;
    while (!(token == Token{BracketToken::CLOSE}) && !tokenizer->IsEnd()) {
        std::shared_ptr<Object> a = Read(tokenizer);
        if (!tokenizer->IsEnd()) {
            token = tokenizer->GetToken();
        }
        if (!a) {
            tokenizer->Next();
            return std::make_shared<Cell>();
        }
        if (Is<Symbol>(a)) {
            if ("." == As<Symbol>(a)->GetName()) {
                if (tokenizer->GetToken() == Token{BracketToken::OPEN}) {
                    ptr->SetSecond(ReadList(tokenizer));
                    if (!tokenizer->IsEnd()) {
                        token = tokenizer->GetToken();
                    }
                    ++size;
                    continue;
                }
                if (token.index() == 0) {
                    ConstantToken t = std::get<ConstantToken>(token);
                    tokenizer->Next();
                    if (!tokenizer->IsEnd()) {
                        token = tokenizer->GetToken();
                    }
                    Number n;
                    n.Set(t.value);
                    ptr->SetSecond(std::make_shared<Number>(n));
                    ++size;
                    continue;
                }
                throw SyntaxError("");
            }
            if (size != 0) {
                ptr->SetSecond(std::make_shared<Cell>());
                ptr = As<Cell>(ptr->GetSecond());
                ptr->SetFirst(a);
                ++size;
                continue;
            }
            ptr->SetFirst(a);
            ++size;
            continue;
        }
        if (Is<Number>(a) || Is<Cell>(a)) {
            if (size == 0) {
                ptr->SetFirst(a);
                ++size;
                continue;
            }
            if (ptr->GetSecond() != nullptr) {
                throw SyntaxError("");
            }
            ptr->SetSecond(std::make_shared<Cell>());
            ptr = As<Cell>(ptr->GetSecond());
            ptr->SetFirst(a);
            ++size;
            continue;
        }
        throw SyntaxError("");
    }
    if (tokenizer->IsEnd()) {
        throw SyntaxError("");
    }
    tokenizer->Next();
    return size > 0 ? ans : nullptr;
}

bool IsReal(std::shared_ptr<Object> a, Tokenizer* tokenizer) {
    if (a == nullptr) {
        tokenizer->Next();
        return false;
    }
    return true;
}