#include <array>
#include <cassert>
#include <cstring>
#include <orb/parser/cursor.hpp>

#define ORB_ASSERT assert


struct Test
{
    int len;
    const char* cstr;

    Test(const char* cstr)
        : len(strlen(cstr)), cstr(cstr)
    {}

    Test(const Test& other)
        : len(other.len)
    {
        auto ptr = new char[len];

        std::strcpy(ptr, other.cstr);
        cstr = ptr;
    }

    Test(Test&& other)
        : len(std::move(other.len)), cstr(std::move(other.cstr))
    {
        other.cstr = new char('\0');
        other.len  = 0;
    }

    ~Test() { delete cstr; }
};

int main()
{
    auto arr  = std::array<int, 6> {1, 2, 3, 4, 5, 6};
    auto curs = orb::Cursor<int>(std::move(arr));

    int value      = **curs.Next().Next().Next().Eat(); // value is 4
    const int* ptr = *curs.Next().Eat();                // value is pointer to 6
                                                        // curs has no more values
    assert(value == 4);
    assert(*ptr == 6);
    assert(!curs.IsValid());
    assert(curs.IsValid() == curs.Get().has_value());

    auto newArr  = std::array<Test, 6> {"1", "2", "3", "4", "5", "6"};
    auto testArr = std::array<Test, 6>(newArr);

    auto newCurs = orb::Cursor<Test>(std::move(newArr));

    for (auto&& elem : newArr)
        assert(*elem.cstr == '\0');

    for (size_t i = 0; i < testArr.size(); ++i)
    {
        const auto& test = testArr[i];
        const auto& real = **newCurs.Eat();

        assert(std::strcmp(test.cstr, real.cstr) == 0);
    }

    exit(0);
}
