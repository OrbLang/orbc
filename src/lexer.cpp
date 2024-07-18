#include "lexer.hpp"

#include "tokens.hpp"

#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

using namespace Parser;

Lexer::Lexer(const char* path)
        : filestream{std::ifstream{path}}, identifier{std::string{""}}, operatorStr{'\0', '\0'}
{
}

void Lexer::SkipWhitespace()
{
    while (isspace(filestream.peek()))
        filestream.get();
}

bool Lexer::NextIdentifier()
{
    SkipWhitespace();
    char nextChar = filestream.peek();

    // If the next identifier is a special character
    if (!isalnum(nextChar) || nextChar == '_')
        return false;

    identifier = filestream.get();
    while (isalnum(filestream.peek()))
        identifier += filestream.get();

    return true;
}

Tokens::TokenType Lexer::NextToken()
{
    using namespace Tokens;
    SkipWhitespace();

    if (isalpha(filestream.peek()))
    {
        NextIdentifier();
        if (identifier == "create")
        {
            if (!NextIdentifier())
                std::cerr << "Invalid character after 'create' keyword: \"" << filestream.peek()
                          << '\"';

            if (identifier == "spell")
                return TokenType::CreateFunc;

            return TokenType::CreateVar;
        }
    }
}

std::optional<Ast::Operator> Lexer::GetOperator() {}
std::optional<std::string> Lexer::GetIdentifier() {}
