#pragma once

#include "ast.hpp"
#include "tokens.hpp"

#include <fstream>
#include <optional>


namespace Parser
{
class Lexer
{
    std::ifstream filestream;
    std::string identifier;
    char operatorStr[2];

public:
    Lexer(const char* path);

    Tokens::TokenType NextToken();
    std::optional<Ast::Operator> GetOperator();
    std::optional<std::string> GetIdentifier();

private:
    bool NextIdentifier();
    void SkipWhitespace();
};

} // namespace Parser
