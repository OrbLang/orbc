const std = @import("std");
const AnyReader = std.io.AnyReader;

const Lexer = @This();

file_reader: AnyReader,

pub const Token = union(enum) {
    pub const Literal = union(enum) {
        string: []u21,
        uint: usize,
        int: isize,
    };
    pub const Keyword = enum {
        create,
        spell,
        cast,
    };
    pub const Bracket = enum {
        /// (
        lparen,
        /// )
        rparen,
        /// {
        lcurly,
        /// }
        rcurly,
        /// [
        lsquare,
        /// ]
        rsquare,
    };
    pub const Operator = enum {
        /// ==
        eq,
        /// !=
        neq,
        /// >
        gt,
        /// <
        lt,
        /// >=
        gte,
        /// <=
        lte,
    };

    literal: Literal,
    keyword: Keyword,
    bracket: Bracket,
    operator: Operator,
    identifier: []u21,
};

pub fn init(reader: AnyReader) Lexer {
    return .{
        .file_reader = reader,
    };
}

pub fn next(self: Lexer) ?Token {
    _ = self;
    return null;
}
