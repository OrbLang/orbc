#include "ast.hpp"
#include "lexer.hpp"

namespace parser
{
ast::BlockNode ParseToAST(const char* path)
{
    // std::vector<ast::BlockNode> traceback;

    Lexer lexer(path);

    Lexer::TokenData nextToken = lexer.NextToken();

    // return traceback[0];
}

} // namespace parser
