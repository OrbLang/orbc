#include <ostream>
#include <ppltasks.h>
namespace logging
{

namespace color
{
enum class Color
{
    FgRed = 31,
    FgGreen = 32,
    FgYellow = 33,
    FgBlue = 34,
    FgDefault = 39,
    BgRed = 41,
    BgGreen = 42,
    BgYellow = 43,
    BgBlue = 44,
    BgDefault = 49
};

enum class Effect
{
    None = 0,
    Bold = 1,
    Dim = 2,
    Italic = 3,
    Underline = 4,
    Blinking = 5,
    Inverse = 7,
    Hidden = 8,
    StrikeThrough = 9,

    NoBold = 22,
    NoDim = 22,
    NoItalic = 23,
    NoUnderline = 24,
    NoBlinking = 25,
    NoInverse = 27,
    NoHidden = 28,
    NoStrikeThrough = 29
};

struct Modifier
{
    Color color;
    Effect effect;

    friend std::ostream& operator<<(std::ostream& os, const Modifier& mod)
    {
        int colorCode = static_cast<int>(mod.color);
        int effectCode = static_cast<int>(mod.effect);

        if (effectCode == 0)
            return os << "\x1b[" << colorCode << "m";

        return os << "\x1b[" << effectCode << ";" << colorCode << "m";
    }
};
}; // namespace color
} // namespace logging
