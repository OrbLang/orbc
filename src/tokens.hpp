#pragma once

namespace parser
{

enum class TokenType
{
    Invalid,
    Keyword,
    Operator,
    Identifier,
    Number,
    Bracket,
};

enum class Keywords
{
    Create,
    Spell,
    Cast,
    /*
    Artifact,
    Enchant,
    Scroll,
    Codex,
    */
};

} // namespace parser
