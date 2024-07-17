#include "ast.hpp"


bool isExpr(Parser::Ast::ExprType exprType)
{
    switch (exprType)
    {
    case Parser::Ast::ExprType::Head:
        return false;
        break;
    case Parser::Ast::ExprType::BinOp:
        return true;
        break;
    case Parser::Ast::ExprType::Cmp:
        return true;
        break;
    case Parser::Ast::ExprType::Assign:
        return false;
        break;
    case Parser::Ast::ExprType::Identifier:
        return true;
        break;
    case Parser::Ast::ExprType::ConstantNum:
        return true;
        break;
    case Parser::Ast::ExprType::Call:
        return true;
        break;
    }
}
