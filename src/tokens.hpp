namespace Parser
{
namespace Tokens
{
enum class TokenType
{
    /// Declare a variable.
    /// Holds an identifier.
    VarDecl,
    /// Assign a value to a variable.
    /// Holds an identifier and an Expression.
    Assign,
    /// Creates a function.
    /// Holds an identifier, list of params, and a block to execute.
    FuncDecl,
    /// Calls a function.
    /// Holds an identifier, and a list of expressions to use as params.
    FuncCall,
    /// Makes an if statement.
    /// Holds an expression to check, as well as a block to execute.
    If,
};
} // namespace Tokens

struct Token
{
    Tokens::TokenType type;
};
} // namespace Parser
