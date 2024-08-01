#include "lexer.hpp"

#include "ast.hpp"
#include "tokens.hpp"

#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

using namespace Parser;

Lexer::Lexer(const char* path)
        : filestream{std::ifstream{path}}, identifier{std::string{""}}, op{Ast::Operator::Add},
          prevTokPos(filestream.tellg())
{
}

void Lexer::SkipWhitespace()
{
    prevTokPos = filestream.tellg();
    while (isspace(filestream.peek()))
        filestream.get();
}

// Needs to return Error
bool Lexer::NextIdentifier()
{
    SkipWhitespace();
    char nextChar = filestream.peek();

    // If the next identifier is a special character
    if (!isalpha(nextChar) || nextChar == '_')
        return false;

    identifier = filestream.get();
    nextChar = filestream.peek();
    while (isalnum(nextChar) || nextChar == '_')
        identifier += filestream.get();

    return true;
}

// Needs to return error
bool Lexer::NextNumber()
{
    using namespace types;
    auto IsValidNumberChar = [](const char c) { return isdigit(c) || c == '.'; };

    if (!IsValidNumberChar(filestream.peek()))
        return false;

    std::string numberStr{};
    numberStr = filestream.get();

    type = numberStr == "." ? Type::Flow : Type::Rune;
    while (IsValidNumberChar(filestream.peek()))
    {
        const char currChar = filestream.get();
        // Return error: too many `.` characters in number
        if (currChar == '.' && type == Type::Flow)
            return false;

        // First `.` character found
        if (currChar == '.' && type == Type::Rune)
            type = Type::Flow;

        numberStr += currChar;
    }

    // Return error: Invalid symbol in number
    if (!isspace(filestream.peek()))
        return false;

    // If number begins with a `.` insert a 0 in front
    if (numberStr.front() == '.')
        numberStr.insert(numberStr.begin(), 1, '0');

    // If number ends with a `.` insert a 0 in the back
    if (numberStr.back() == '.')
        numberStr += '0';

    // Note: Technically this means that `.` is a valid number
    // since `.` => `0.0`
    // I'm keeping it

    // Add more type detection in the future (With specific types like R8 and F64)
    if (type == Type::Rune)
        number = stoi(numberStr);
    else if (type == Type::Flow)
        number = stof(numberStr);

    return true;
}

// Needs to return error
/* bool Lexer::NextOperator() */
/* { */
/*     SkipWhitespace(); */
/*     char nextChar = filestream.peek(); */
/* } */

// Needs to return error
Lexer::TokenData Lexer::NextToken()
{
    SkipWhitespace();
    /**/
    /* // Identifiers like keywords and variable / function names */
    /* if (NextIdentifier()) */
    /* { */
    /*     if (identifier == "cast") */
    /*         return TokenType::CallFunc; */
    /**/
    /*     if (identifier == "create") */
    /*     { */
    /*         if (!NextIdentifier()) */
    /*             std::cerr << "Invalid character after 'create' keyword: \"" << filestream.peek()
     */
    /*                       << '\"'; */
    /**/
    /*         if (identifier == "spell") */
    /*             return TokenType::CreateFunc; */
    /**/
    /*         // We want a seperate call to get the Identifier */
    /*         // hence we roll back the filestream to the `create` keyword */
    /*         filestream.seekg(prevTokPos); */
    /**/
    /*         return TokenType::CreateVar; */
    /*     } */
    /**/
    /*     return TokenType::Identifier; */
    /* } */
    /**/
    /* // Integers and floats */
    /* if (NextNumber()) */
    /*     return TokenType::Constant; */
}

Ast::Operator Lexer::GetOperator() {}
std::string Lexer::GetIdentifier() {}
