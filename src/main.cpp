#include <iostream>

int main()
{
    std::cout << "Hello World";

    // Doesn't work? The constructor is for some reason deleted

    const char* str = "hello let me be frank ";
}
