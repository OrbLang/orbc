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
    LogicalNot,
    /// Same as `&&`
    LogicalAnd,
    /// Same as `||`
    LogicalOr,
    /// Same as `^^` (New operator)
    LogicalXor,
    /// Same as `<<`
    BitShiftL,
    /// Same as `>>`
    BitShiftR,
    /// Same as `|`
    BitOr,
    /// Same as `&`
    BitAnd,
    /// Same as `^`
    BitXor,
    /// Same as `~`
    BitNot,
};

class AstToken
{
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
    Operator op;
    // The left hand side value.
    std::unique_ptr<ExprToken> lhs;
    // THe right hand side value.
    std::unique_ptr<ExprToken> rhs;
};

// Holds a constant number.
class NumberToken : ExprToken
{
private:
    // The constants value. E.g. `5`.
    double number;
};

// Holds an identifier for a variable.
class VariableToken : ExprToken
{
private:
    // The variables name/identifier. E.g. `foo`.
    const char* identifier;
};

// Calls a function.
class FuncCallToken : ExprToken
{
private:
    // The name/identifier of the function.
    const char* identifier;
    // The parameters.
    const std::unique_ptr<ExprToken>* paramExprs;
    // The number of parameters passed.
    int paramCount;
};

class StatementToken
{
};

// Holds a block of tokens. Both `ExprTokens` and `StatementToken` are valid.
class BlockToken : StatementToken
{
private:
    // A list of tokens to run from left to right, therefore First in, first out.
    const std::unique_ptr<AstToken>* tokens;
    // The amount of tokens in the token list.
    int tokenCount;

public:
    // Push a token to the block.
    void PushTok(std::unique_ptr<AstToken> token);
};

// Declare a new variable.
class VariableDeclToken : StatementToken
{
private:
    // The name/identifier of the new variable.
    const char* identifier;
    // The variables type.
    const char* type;
    // If the variable is assigned a value.
    bool isAssigned;
    // The expression that the variable is assigned with, if `isAssigned = true`.
    std::unique_ptr<ExprToken> value;
};

// Declare a new function.
class FunctionDeclToken : StatementToken
{
private:
    // The name/identifier of the new variable.
    const char* identifier;
    // The type that the function will return.
    const char* returnType;
    // The identifiers of the functions parameters.
    const char** paramIdents;
    // The types of the functions parameters.
    const char** paramTypes;
    // The block which holds the tokens in the function.
    std::unique_ptr<BlockToken> block;
};

// Create an if statement.
class IfToken : StatementToken
{
private:
    // The condition.
    std::unique_ptr<ExprToken> condition;
    // The block which is run if the condition is evaluated to true.
    std::unique_ptr<BlockToken> block;
};

// Create a while loop.
class WhileLoopToken : StatementToken
{
private:
    // The run condition.
    std::unique_ptr<ExprToken> condition;
    // The block which is run for each iteration.
    std::unique_ptr<BlockToken> block;
};


}; // namespace Ast
}; // namespace Parser
