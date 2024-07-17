#include "parser.hpp"

#include <iostream>

int main()
{
    std::cout << "Hello World";

    // Doesn't work? The constructor is for some reason deleted
    Parser::TokenParser parser = Parser::TokenParser();

    const char* str = "hello let me be frank ";

    for (int i = 0; i < 21; i++)
    {
        parser.ParseNext(str[i], str[i + 1]);
    };
}
