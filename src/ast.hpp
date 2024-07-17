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

// ExprToken Interface;
// An ExprTken is a token that can be evaluated to a value.
// Like `5 + 5` can be evaluated to `10`.
// Statements do not lie under here, as `int foo;` can't be evaluated to a number.
class ExprToken
{
public:
    // Holds the type of the expressions value, e.g. `int`, `Entry::Foo`.
    const char* type;
    // Generates an LLVM value for the expression token.
    // virtual Value GetIRValue() = 0;
    // Commented as we do not have the LLVM librabry, which
    // includes Value, imported yet.
};

// Performs an operator on two expressions (`lhs` & `rhs`). E.g. `lhs + rhs`
class OperatorToken : ExprToken
{
private:
    // The operator, like `>`, `||`, `-`, and such.
    Operator _op;
    // The left hand side value.
    std::unique_ptr<ExprToken> _lhs;
    // THe right hand side value.
    std::unique_ptr<ExprToken> _rhs;
};

// Holds a constant number.
class NumberToken : ExprToken
{
private:
    // The constants value. E.g. `5`.
    double _number;
};

// Holds an identifier for a variable.
class VariableToken : ExprToken
{
private:
    // The variables name/identifier. E.g. `foo`.
    const char* _identifier;
};

// Calls a function.
class FuncCallToken : ExprToken
{
private:
    // The name/identifier of the function.
    const char* _identifier;
    // The parameters.
    const ExprToken* _paramIdents;
    // The number of parameters passed.
    int _paramCount;
};

}; // namespace Ast
}; // namespace Parser
