#pragma once

#include <cstdint>
#include <string>
namespace Parser
{
namespace types
{

typedef int32_t rune;
typedef char glyph;
typedef std::string chant;
typedef float flow;
typedef bool sigil;

typedef int8_t r8;
typedef int16_t r16;
typedef int32_t r32;
typedef int64_t r64;

typedef float f32;
typedef double f64;
typedef long double f128;


enum class OrbType
{
    // Integers

    /// i8
    R8,
    /// i16
    R16,
    /// i32
    R32,
    /// Standard Integer
    Rune = R32,
    /// i64
    R64,

    /// f32
    F32,
    /// Standard Floating Point
    Flow = F32,
    /// f64
    F64,
    /// f128
    F128,

    /// Character
    Glyph,
    /// String
    Chant,
    /// Boolean
    Sigil,

    // Special types
    /// Class
    Artifact,
    /// Interface
    Scroll,
    /// Enumerator
    Codex,
};

} // namespace types
} // namespace Parser
