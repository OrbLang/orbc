#pragma once

#include <memory>
#include <string>

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
    std::string name;
    Operator op;
};

class AstToken
{
   public:
    ExprType type;
    ExprValue value;
    std::shared_ptr<AstToken> lhs;
    std::shared_ptr<AstToken> rhs;

    /// Create a HEAD token
    AstToken();

    /// Create a new AstToken with a defined type and value
    template <typename T>
    AstToken(ExprType tokenType, T tokenValue);
};
}  // namespace Ast
};  // namespace Parser
