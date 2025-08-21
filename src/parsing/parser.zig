const std = @import("std");
const parsing = @import("parsing");
const ErrorBuilder = @import("./../ErrorBuilder.zig");

pub const ParseError = error{} || std.mem.Allocator.Error;

pub fn parse(allocator: std.mem.Allocator, lexer: parsing.Lexer, errb: *ErrorBuilder) ParseError!parsing.AstRoot {
    const ctx: ParseCtx = .{ .allocator = allocator, .lexer = &lexer, .errb = errb };

    var nodes = std.ArrayList(parsing.AstNode).init(allocator);
    defer nodes.deinit();

    while (lexer.next()) |token| {
        const node = try parseStatement(token, ctx);

        if (!node.inner.isTopLevelDef())
            errb.append(lexer.getLoc(), "Expected top level definition.");

        nodes.append(node);
    }

    return .{
        .allocator = allocator,
        .nodes = try nodes.toOwnedSlice(),
    };
}

fn parseStatement(token: parsing.Token, ctx: ParseCtx) ParseError!parsing.AstNode {
    _ = token;
    _ = ctx;
}

fn parseExpr(token: parsing.Token, ctx: ParseCtx) ParseError!parsing.AstNode {
    _ = token;
    _ = ctx;
}

const ParseCtx = struct {
    allocator: std.mem.Allocator,
    lexer: *parsing.Lexer,
    errb: *ErrorBuilder,
};
