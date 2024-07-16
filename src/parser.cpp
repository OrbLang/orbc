#include "parser.hpp"

#include <iostream>

Parser::TokenParser::TokenParser() : _tokenTree{Ast::AstToken()} {}

void Parser::TokenParser::ParseNext(char currChar, char nextChar)
{
    if (currChar == ' ')
    {
        if (_currBuffer == "let")
        {
            std::cout << "GOT LET!!!" << std::endl;
        }
    }
    _currBuffer.push_back(currChar);
}
