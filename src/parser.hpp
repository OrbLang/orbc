#pragma once

#include "ast.hpp"

#include <string>

namespace Parser
{
enum class Status
{
    InExpr,
};

class TokenParser
{
private:
    std::string _currBuffer;
    Ast::AstNode _tokenTree;

public:
    TokenParser();

    void ParseNext(char currChar, char nextChar);

    Ast::AstNode GetAST();
};

} // namespace Parser
