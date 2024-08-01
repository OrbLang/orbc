#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

typedef std::string ident_t;

namespace parser
{
namespace ast
{


/// Holds an operator, like `+`, `-`, `>=`, `&`, and `.`.
enum class Operator
{
    /*  MATH  */
    /// Same as `LHS + RHS`
    Add,
    /// Same as `LHS - RHS`
    Sub,
    /// Same as `LHS * RHS`
    Mult,
    /// Same as `LHS / RHS`
    Div,
    /// Same as `LHS % RHS`
    Mod,
    /// Same as `-LHS`
    Minus,

    /*  COMPARISON / LOGICAL */
    /// Same as `LHS > RHS`
    GreaterThan,
    /// Same as `LHS >= RHS`
    GreaterEqual,
    /// Same as `LHS < RHS`
    LessThan,
    /// Same as `LHS <= RHS`
    LessEqual,
    /// Same as `LHS == RHS`
    Equal,
    /// same as `LHS != RHS`
    NotEqual,
    /// Same as `!LHS`
    LogicalNot,
    /// Same as `LHS && RHS`
    LogicalAnd,
    /// Same as `LHS || RHS`
    LogicalOr,
    /// Same as `LHS ^^ RHS` (New operator: Logical version og `^`)
    LogicalXor,

    /*  BITWISE  */
    /// Same as `LHS << RHS`
    BitShiftL,
    /// Same as `LHS >> RHS`
    BitShiftR,
    /// Same as `LHS | RHS`
    BitOr,
    /// Same as `LHS & RHS`
    BitAnd,
    /// Same as `LHS ^ RHS`
    BitXor,
    /// Same as `~LHS`
    BitNot,

    /*  SYNTAX  */
    /// Same as `LHS.RHS`
    MemberAccess,
    /// Same as `LHS->RHS`
    PointerMemberAccess,
    /// Same as `LHS::RHS`
    StaticMemberAccess,
    /// Same as `*LHS`
    Dereference,
    /// Same as `&LHS`
    Address
};



/// A map that sorts the order of operations. A higher weight should be evaluated before a lower
/// value. Example:
/// * `2 + 5 * 4`
///     * `Operator::Add` has a weight of `0`
///     * `Operator::Mult` has a weight of `10`
/// * Therefore the expression will evaluate to: `2 + (5 * 4)`
const std::unordered_map<Operator, uint8_t> operatorOrder = {
    {          Operator::LogicalOr, 0},
    {         Operator::LogicalXor, 0},
    {         Operator::LogicalAnd, 1},
    {              Operator::Equal, 2},
    {           Operator::NotEqual, 2},
    {        Operator::GreaterThan, 3},
    {       Operator::GreaterEqual, 3},
    {           Operator::LessThan, 3},
    {          Operator::LessEqual, 3},
    {              Operator::BitOr, 4},
    {             Operator::BitXor, 4},
    {             Operator::BitAnd, 4},
    {          Operator::BitShiftL, 5},
    {          Operator::BitShiftR, 5},
    {                Operator::Add, 6},
    {                Operator::Sub, 6},
    {               Operator::Mult, 7},
    {                Operator::Div, 7},
    {                Operator::Mod, 7},
    {        Operator::Dereference, 8},
    {            Operator::Address, 8},
    {             Operator::BitNot, 8},
    {         Operator::LogicalNot, 8},
    {              Operator::Minus, 8},
    {       Operator::MemberAccess, 9},
    {Operator::PointerMemberAccess, 9},
    { Operator::StaticMemberAccess, 9},
};


class AstNode
{
    ~AstNode() = default;
    AstNode(const AstNode& obj) = default;
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
class LiteralValueNode : ExprNode
{
public:
    typedef std::variant<int, float, const char*, char, bool> literal_t;

private:
    // The constants value. E.g. `5`.
    literal_t value;
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
    std::vector<std::unique_ptr<ExprNode>> paramExprs;
    // The number of parameters passed.
    int paramCount;
};

class StatementNode : public AstNode
{
};

/// Holds a block of nodes. Both `ExprNode` and `StatementNode` are valid.
class BlockNode : public StatementNode
{
private:
    /// A list of nodes to run from left to right, therefore First in, first out.
    std::vector<std::unique_ptr<AstNode>> nodes;

public:
    /// Create a `BlockNode` from a list of `AstNode`'s
    BlockNode(std::unique_ptr<AstNode> nodes);
    /// Push a node to the block.
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
    std::vector<const char*> paramIdents;
    // The types of the functions parameters.
    std::vector<const char*> paramTypes;
    // The block which holds the nodes in the function.
    std::unique_ptr<BlockNode> block;
};

// Create an if statement.
class IfNode : public StatementNode
{
private:
    // The if statements conditions
    std::vector<std::unique_ptr<ExprNode>> conditions;
    // The blocks which is run if it's condition is evaluated to true.
    std::vector<std::unique_ptr<BlockNode>> blocks;
};

// A chained If Else statement.
class FullIfNode : public StatementNode
{
private:
    // The if statements conditions
    std::vector<std::unique_ptr<ExprNode>> conditions;
    // The blocks which is run if it's condition is evaluated to true.
    std::vector<std::unique_ptr<BlockNode>> blocks;
    // The block of code that will run if none of the conditions are set to true.
    std::unique_ptr<BlockNode> elseBlock;
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

BlockNode ParseTokens();

}; // namespace ast
}; // namespace parser
