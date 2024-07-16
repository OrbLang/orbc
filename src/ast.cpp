#include "ast.hpp"

template <typename T>
Parser::Ast::AstToken::AstToken(ExprType exprType, T tokenValue)
{
    type = exprType;
    switch (type)
    {
        case ExprType::Head:
            // Set `value` to zero, since its not needed
            value.number = 0.0;
            break;
        case ExprType::BinOp:
            value.op = tokenValue;
            break;
        case ExprType::Cmp:
            value.op = tokenValue;
            break;
        case ExprType::Assign:
            // Set `value` to zero, since its not needed
            value.number = 0.0;
            break;
        case ExprType::Identifier:
            value.name = tokenValue;
            break;
        case ExprType::ConstantNum:
            value.number = tokenValue;
            break;
        case ExprType::Call:
            // Set `value` to zero, since its not needed
            value.number = 0.0;
            break;
    };
}

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
