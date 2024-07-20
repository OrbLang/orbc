#pragma once

#include "ast.hpp"
#include "tokens.hpp"
#include "types.hpp"

#include <climits>
#include <fstream>
#include <variant>


namespace Parser
{

class Lexer
{
public:
    typedef std::variant<int8_t, int16_t, int32_t, int64_t, float, double, long double>
        numberVariant;

private:
    std::ifstream filestream;
    std::string identifier;
    numberVariant number;
    types::OrbType type;
    Ast::Operator op;
    std::ifstream::pos_type prevTokPos;

public:
    Lexer(const char* path);

    Tokens::TokenType NextToken();
    Ast::Operator GetOperator();
    std::string GetIdentifier();
    types::OrbType GetType();
    numberVariant GetNumber();


private:
    void SkipWhitespace();
    bool NextIdentifier();
    bool NextNumber();
    /* bool NextOperator(); */
};

} // namespace Parser
