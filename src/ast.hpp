#pragma once

#include <memory>
#include <string>

typedef std::string ident_t;

namespace Parser
{
namespace Ast
{
/// Holds an operator, like `+`, `-`, `>=`, `&`, and `.`.
enum class Operator
{
    /*  MATH  */
    /// Same as `+`
    Add,
    /// Same as `-`
    Sub,
    /// Same as `*`
    Mult,
    /// Same as `/`
    Div,
    /// Same as `%`
    Mod,

    /*  COMPARISON / LOGICAL */
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

    /*  BITWISE  */
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

class AstNode
{
    // virtual ~AstNode() = default;
};

// ExprNode Interface;
// An ExprTken is a node that can be evaluated to a value.
// Like `5 + 5` can be evaluated to `10`.
// Statements do not lie under here, as `int foo;` can't be evaluated to a number.
class ExprNode : public AstNode
{
public:
    // Holds the type of the expressions value, e.g. `int`, `Entry::Foo`.
    const char* type;
    // Generates an LLVM value for the expression node.
    // virtual Value GetIRValue() = 0;
    // Commented as we do not have the LLVM librabry, which
    // includes Value, imported yet.
};

// Performs an operator on two expressions (`lhs` & `rhs`). E.g. `lhs + rhs`
class OperatorNode : public ExprNode
{
private:
    // The operator, like `>`, `||`, `-`, and such.
    Operator op;
    // The left hand side value.
    std::unique_ptr<ExprNode> lhs;
    // THe right hand side value.
    std::unique_ptr<ExprNode> rhs;
};

// Holds a constant number.
class NumberLiteralNode : ExprNode
{
private:
    // The constants value. E.g. `5`.
    double number;
};

// Holds a constant String.
class StringLiteralNode : public ExprNode
{
private:
    // The constant string. E.g. `"Hello, World!"`.
    const char* string;
};

// Holds a constant char.
class CharLiteralNode : public ExprNode
{
private:
    // The constant char. E.g. `'c'`.
    char charachter;
};

// Holds an identifier for a variable.
class VariableNode : public ExprNode
{
private:
    // The variables name/identifier. E.g. `foo`.
    const char* identifier;
};

// Calls a function.
class FuncCallNode : public ExprNode
{
private:
    // The name/identifier of the function.
    const char* identifier;
    // The parameters.
    const std::unique_ptr<ExprNode>* paramExprs;
    // The number of parameters passed.
    int paramCount;
};

class StatementNode : public AstNode
{
};

// Holds a block of nodes. Both `ExprNode` and `StatementNode` are valid.
class BlockNode : public StatementNode
{
private:
    // A list of nodes to run from left to right, therefore First in, first out.
    const std::unique_ptr<AstNode>* nodes;
    // The amount of nodes in the nodes list.
    int nodeCount;

public:
    // Push a node to the block.
    void PushTok(std::unique_ptr<AstNode> node);
};

// Declare a new variable.
class DeclVariableNode : public StatementNode
{
private:
    // The name/identifier of the new variable.
    const char* identifier;
    // The variables type.
    const char* type;
    // If the variable is assigned a value.
    bool isAssigned;
    // The expression that the variable is assigned with, if `isAssigned = true`.
    std::unique_ptr<ExprNode> value;
};

// Assign/Reassign a value to a variable.
class AssignVariableNode : public StatementNode
{
private:
    // The name/identifier of the new variable.
    const char* identifier;
    // The expression that the variable is assigned to.
    std::unique_ptr<ExprNode> value;
};

// Declare a new function.
class DeclFunctionNode : public StatementNode
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
    // The block which holds the nodes in the function.
    std::unique_ptr<BlockNode> block;
};

// Create an if statement.
class IfNode : public StatementNode
{
private:
    // The condition.
    std::unique_ptr<ExprNode> condition;
    // The block which is run if the condition is evaluated to true.
    std::unique_ptr<BlockNode> block;
};

// Create a while loop.
class WhileLoopNode : public StatementNode
{
private:
    // The run condition.
    std::unique_ptr<ExprNode> condition;
    // The block which is run for each iteration.
    std::unique_ptr<BlockNode> block;
};

}; // namespace Ast
}; // namespace Parser
