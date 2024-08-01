#include "ast.hpp"
#include "lexer.hpp"
#include "tokens.hpp"

#include <iostream>
#include <vector>

namespace parser
{
ast::BlockNode ParseToAST(const char* path)
{
    // std::vector<ast::BlockNode> traceback;

    Lexer lexer(path);

    tokens::TokenType nextToken = lexer.NextToken();
    while (nextToken != tokens::TokenType::Eof)
    {
    }

    // return traceback[0];
}

} // namespace parser
