#pragma once

#include <cstring>
#include <memory>
#include <string>
#include <typeinfo>

typedef std::string ident_t;

namespace Parser
{
namespace Ast
{

enum class ExprType
{
    /// The head of the tree, or the entry point
    /// * `lhs` - nullptr
    /// * `rhs` - nullptr
    Head,
    /// An operator, like +, -, /. E.g. `lhs + rhs`
    /// * `lhs` - The first expression
    /// * `rhs` - The second expression
    BinOp,
    /// Compare the lhs and rhs with an operator, like == or <. E.g.
    /// `lhs < rhs`
    /// * `lhs` - The first expression to compare
    /// * `rhs` - The second expression to compare
    Cmp,
    /// Assign a variable.
    /// * `lhs` - The variable name, though `TokenType::Identifier`
    /// * `rhs` - The variables data
    Assign,
    /// Holds the identifier of an object or variable
    /// * `value` - Name
    Identifier,
    /// A constant number
    /// * `value` - The value
    ConstantNum,
    /// Calls a function
    /// * `lhs` - The functions identifier
    /// * `rhs` - The functions arguments
    Call,
};

bool isExpr(ExprType tokenType);
enum class Operator
{
    /// Same as `+`
    Add,
    /// Same as `-`
    Sub,
    /// Same as `*`
    Mult,
    /// Same as `/`
    Div,
    /// Same as `>`
    GreaterThan,
    /// Same as `>=`
    GreaterEqual,
    /// Same as `<`
    LessThan,
    /// Same as `<=`
    LessEqual,
    /// Same as `==`
    Equal,
    /// Same as `!`
    Not,
};

/// The value behind an AST token.
union ExprValue
{
    double number;
    const char* const name;
    Operator op;
};

class AstToken
{
public:
    ExprType type;
    ExprValue value;
    std::shared_ptr<AstToken> lhs;
    std::shared_ptr<AstToken> rhs;

    AstToken() = default;

    /// Create a new AstToken with a defined type and value
    template <typename T>
    AstToken(ExprType exprType, T tokenValue)
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
};
} // namespace Ast
} // namespace Parser
