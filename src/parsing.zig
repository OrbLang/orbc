pub const Lexer = @import("parsing/Lexer.zig");
pub const parse = @import("parsing/parser.zig").parse;
pub const Token = @import("parsing/Lexer.zig").Token;
pub const AstNode = @import("parsing/AstNode.zig");
pub const AstRoot = AstNode.AstRoot;

test {
    @import("std").testing.refAllDecls(@This());
}
