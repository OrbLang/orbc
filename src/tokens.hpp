#pragma once

namespace parser
{
namespace tokens
{
enum class TokenType
{
    /// A variable, function or class name
    Identifier,
    /// Declare a variable.
    /// Holds an identifier.
    CreateVar,
    /// Creates a function.
    /// Holds an identifier, list of params, and a block to execute.
    CreateFunc,
    /// Assign a value to a variable.
    /// Holds an identifier and an Expression.
    Operator,
    /// Calls a function.
    /// Holds an identifier, and a list of expressions to use as params.
    CallFunc,
    /// A statement fx. If, Else, While...
    /// If it has a condition it also holds a body
    Statement,
    /// A type like an integer or floating point
    Type,
    /// A number like 4 or 3.14
    Constant,
    /// Indicating the end of a file
    Eof,
};

} // namespace tokens

struct Token
{
    tokens::TokenType type;
};
} // namespace parser
