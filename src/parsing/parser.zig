const std = @import("std");
const parsing = @import("../parsing.zig");
const log = @import("../log.zig");
const ErrorBuilder = @import("../ErrorBuilder.zig");

const Lexer = parsing.Lexer;
const Token = parsing.Token;
const AstNode = parsing.AstNode;
const AstRoot = parsing.AstRoot;
const TypeInfo = AstNode.TypeInfo;

const ParseCtx = struct {
    allocator: std.mem.Allocator,
    lexer: *Lexer,
    errb: *ErrorBuilder,
};

pub const ParseError = error{
    GotEof,
    UnexpectedToken,
    ExpectedTopLevelDef,
    MismatchedBrackets,
    MissingEol,
} || std.mem.Allocator.Error;

pub fn parse(allocator: std.mem.Allocator, lexer: *Lexer, errb: *ErrorBuilder) ParseError!AstRoot {
    const ctx: ParseCtx = .{ .allocator = allocator, .lexer = lexer, .errb = errb };

    var gotError = false;

    var nodes = std.ArrayList(AstNode).init(allocator);
    defer nodes.deinit();

    while (lexer.next()) |token| {
        const node = try parseStatement(token, ctx);

        if (!node.inner.isTopLevelDef()) {
            try errb.err(lexer.getLoc(), "Expected top level definition.");
            gotError = true;
        }

        try nodes.append(node);
    }

    if (gotError)
        return error.ExpectedTopLevelDef;

    return .{
        .allocator = allocator,
        .nodes = try nodes.toOwnedSlice(),
    };
}

fn parseStatement(token: Token, ctx: ParseCtx) ParseError!AstNode {
    var expr = switch (token) {
        .keyword => |kw| keyword: {
            switch (kw) {
                .create => {
                    const next_tok = ctx.lexer.next() orelse return error.GotEof;

                    if (isToken(next_tok, .{ .keyword = .spell })) {
                        break :keyword try parseSpellDecl(next_tok, ctx);
                    }
                    // TODO: Cantrips, Codex's, Artifacts
                    break :keyword try parseVarDecl(next_tok, ctx);
                },
                else => break :keyword try parseExpr(token, ctx),
            }
        },
        else => try parseExpr(token, ctx),
    };

    if (isToken(ctx.lexer.peek(), .{ .operator = .eol })) {
        _ = ctx.lexer.next();
        expr.is_statement = true;
        return expr;
    }

    if (ctx.lexer.peek() == null or !expr.inner.needsEol())
        return expr;

    // Allow not to have an eol on the last line in a block
    if (isToken(ctx.lexer.peek(), .{ .bracket = .rcurly })) {
        return expr;
    }

    try ctx.errb.err(ctx.lexer.getLoc(), "Missing ';' after statement");
    return error.MissingEol;
}

fn parseExpr(token: Token, ctx: ParseCtx) ParseError!AstNode {
    // This switch case only handles the starting tokens in an expression,
    // which is why binop operators result in an error
    const node: AstNode = blk: switch (token) {
        .literal => |lit| {
            const ast_lit: AstNode.AstLiteral = switch (lit) {
                .string => |str| str_lit: {
                    var str_builder = std.ArrayList(u21).init(ctx.allocator);
                    defer str_builder.deinit();

                    try str_builder.appendSlice(str);

                    // Append string literals together
                    while (ctx.lexer.peek()) |tok| {
                        const lit_tok = expectTokenType(tok, .literal) catch break;

                        switch (lit_tok.literal) {
                            .string => |s| {
                                try str_builder.appendSlice(s);
                            },
                            else => break,
                        }
                    }

                    break :str_lit .{ .chant = try str_builder.toOwnedSlice() };
                },
                .uint => |_| log.todo(@src(), "Add uint literal support\n", .{}),
                .int => |i| .{ .rune = i },
            };
            break :blk .{ .inner = .{ .literal = ast_lit } };
        },
        .identifier => |ident| {
            break :blk .{ .inner = .{ .read_var = try ctx.allocator.dupe(u21, ident) } };
        },
        .operator => |op| {
            _ = op;
            log.todo(@src(), "Handle unary operators", .{});
        },
        .bracket => |bracket| {
            break :blk switch (bracket) {
                .lparen => log.todo(@src(), "Support function calls", .{}),
                .lcurly => try parseBlock(token, ctx),
                .lsquare => log.todo(@src(), "Support indexing (and possibly list literals)", .{}),
                .rcurly, .rparen, .rsquare => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "Found mismatched closing bracket");
                    return error.UnexpectedToken;
                },
            };
        },
        .keyword => |kw| {
            switch (kw) {
                .cast => {
                    const expr = try parseExpr(ctx.lexer.next() orelse return error.GotEof, ctx);
                    break :blk .{ .inner = .{ .cast = try alloc_node(ctx.allocator, expr) } };
                },
                .create => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "unexpected 'create' keyword. the create keyword can only be used as a statement, not as an expression");
                    return error.UnexpectedToken;
                },
                .spell => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "unexpected 'spell' keyword. To create a spell, you must prefix it with the 'create' keyword");
                    return error.UnexpectedToken;
                },
                .cantrip => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "unexpected 'cantrip' keyword. To create a cantrip, you must prefix it with the 'create' keyword");
                    return error.UnexpectedToken;
                },
                .staunch => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "unexpected 'staunch' keyword");
                    return error.UnexpectedToken;
                },
            }
        },
    };

    return node;
}

fn parseBlock(token: Token, ctx: ParseCtx) ParseError!AstNode {
    // TODO: Check if a label is there
    try expectToken(token, .{ .bracket = .lcurly });

    var nodes: std.ArrayList(AstNode) = .init(ctx.allocator);
    defer nodes.deinit();

    while (ctx.lexer.peek()) |tok| {
        // Leave the block when hitting a `}`
        if (isToken(tok, .{ .bracket = .rcurly })) {
            break;
        }

        // If a `{` appears, it will automatically start a new block, which will handle its closing bracket `}` on its own
        try nodes.append(try parseStatement(ctx.lexer.next().?, ctx));
    }

    // Check that the loop exited by a closing bracket `}`, and not by an eof
    if (!isToken(ctx.lexer.peek(), .{ .bracket = .rcurly })) {
        try ctx.errb.err(ctx.lexer.getLoc(), "Block is missing closing bracket (})");
        return error.MismatchedBrackets;
    }
    _ = ctx.lexer.next();

    return .{
        .inner = .{
            .block = .{ .nodes = try nodes.toOwnedSlice() },
        },
    };
}

fn parseSpellDecl(token: Token, ctx: ParseCtx) ParseError!AstNode {
    // TODO: Get qualifiers
    const qualifiers = &.{};

    const spelltype_tok = try expectOneOf(token, &.{ .{ .keyword = .spell }, .{ .keyword = .cantrip } });
    const spelltype = AstNode.SpellType.fromLexKeyword(spelltype_tok.keyword).?;

    const ident_tok = try expectTokenType(ctx.lexer.next(), .identifier);
    const ident = try ctx.allocator.dupe(u21, ident_tok.identifier);

    try expectToken(ctx.lexer.next(), .{ .bracket = .lparen });

    var params = std.ArrayList(AstNode.IdentTypePair).init(ctx.allocator);
    defer params.deinit();

    if (!isToken(ctx.lexer.next(), .{ .bracket = .rparen })) {
        while (true) {
            const p_ident_tok = try expectTokenType(ctx.lexer.next(), .identifier);
            const p_ident = try ctx.allocator.dupe(u21, p_ident_tok.identifier);

            try expectToken(ctx.lexer.next(), .{ .operator = .type_sep });

            const p_type = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);
            try params.append(.{ .ident = p_ident, .typeinfo = p_type });

            if (!isToken(ctx.lexer.peek(), .{ .operator = .item_sep }))
                break;

            _ = ctx.lexer.next(); // Consume the `,`
        }
    }

    try expectToken(ctx.lexer.next(), .{ .bracket = .rparen });

    // TODO: Should spell types be necessary, even for a void spell?
    try expectToken(ctx.lexer.next(), .{ .operator = .fn_type_sep });

    const ret_type = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);

    const body = try parseBlock(ctx.lexer.next() orelse return error.GotEof, ctx);

    return .{
        .inner = .{
            .decl_spell = .{
                .spelltype = spelltype,
                .qualifiers = qualifiers,
                .ident = ident,
                .params = try params.toOwnedSlice(),
                .ret_type = ret_type,
                .body = try alloc_node(ctx.allocator, body),
            },
        },
    };
}

fn parseVarDecl(token: Token, ctx: ParseCtx) ParseError!AstNode {
    const ident_tok = try expectTokenType(token, .identifier);
    const ident = try ctx.allocator.dupe(u21, ident_tok.identifier);

    var typeinfo: ?TypeInfo = null;
    if (isToken(ctx.lexer.peek(), .{ .operator = .type_sep })) {
        _ = ctx.lexer.next(); // Consume the `@`
        typeinfo = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);
    }

    var value: ?*AstNode = null;
    if (isToken(ctx.lexer.peek(), .{ .operator = .assign })) {
        _ = ctx.lexer.next(); // Consume the `=`
        const value_node = try parseExpr(ctx.lexer.next() orelse return error.GotEof, ctx);
        value = try alloc_node(ctx.allocator, value_node);
    }

    return AstNode{
        .inner = .{
            .decl_var = .{
                .ident = ident,
                .value = value,
                .typeinfo = typeinfo,
                .is_const = false,
            },
        },
    };
}

fn parseTypeInfo(token: Token, ctx: ParseCtx) ParseError!TypeInfo {
    // Primitive/alias
    if (isTokenType(token, .identifier)) {
        // If the token is an identifier, then its either a primitive type, like `rune`, or an alias,
        // which will be resolved later on
        const prim = TypeInfo.Prim.fromStr(token.identifier) catch {
            return .{ .alias = try ctx.allocator.dupe(u21, token.identifier) };
        };

        return .{ .prim = prim };
    }

    // Pointer
    if (isToken(token, .{ .operator = .times })) {
        var is_const = false;
        if (isToken(ctx.lexer.peek(), .{ .keyword = .staunch })) {
            _ = ctx.lexer.next(); // Consume the `staunch`
            is_const = true;
        }

        const inner = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);
        return .{ .ptr = .{
            .ptr = .{
                .inner = try alloc_type(ctx.allocator, inner),
                .is_const = is_const,
            },
        } };
    }

    // Slice/Array
    if (isToken(token, .{ .bracket = .lsquare })) {
        const next_tok = ctx.lexer.next();

        // Slice
        if (isToken(next_tok, .{ .bracket = .rsquare })) {
            var is_const = false;
            if (isToken(ctx.lexer.peek(), .{ .keyword = .staunch })) {
                _ = ctx.lexer.next(); // Consume the `staunch`
                is_const = true;
            }

            const inner = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);
            return .{ .ptr = .{
                .slice = .{
                    .inner = try alloc_type(ctx.allocator, inner),
                    .is_const = is_const,
                },
            } };
        }

        // Array
        // TODO: Consider allowing comptime known constants as array length,
        // or even just comptime known operations, like 2+2
        if (isTokenType(next_tok, .literal)) {
            const len = switch (next_tok.?.literal) {
                .uint => |size| size,
                else => {
                    try ctx.errb.err(ctx.lexer.getLoc(), "Invalid array length descriptor");
                    return error.UnexpectedToken;
                },
            };

            try expectToken(ctx.lexer.next(), .{ .bracket = .rsquare });

            var is_const = false;
            if (isToken(ctx.lexer.peek(), .{ .keyword = .staunch })) {
                _ = ctx.lexer.next(); // Consume the `staunch`
                is_const = true;
            }

            const inner = try parseTypeInfo(ctx.lexer.next() orelse return error.GotEof, ctx);
            return .{ .ptr = .{
                .array = .{
                    .inner = try alloc_type(ctx.allocator, inner),
                    .len = len,
                    .is_const = is_const,
                },
            } };
        }
    }

    try ctx.errb.err(ctx.lexer.getLoc(), "Expected valid type");
    return error.UnexpectedToken;
}

const TokenType = std.meta.FieldEnum(Token);

fn isTokenType(lhs: ?Token, rhs: TokenType) bool {
    if (lhs) |lhs_tok| {
        return @intFromEnum(lhs_tok) == @intFromEnum(rhs);
    }
    return false;
}

fn expectTokenType(got: ?Token, expects: TokenType) ParseError!Token {
    if (got) |got_tok| {
        if (isTokenType(got_tok, expects)) {
            return got_tok;
        } else {
            return error.UnexpectedToken;
        }
    } else {
        return error.GotEof;
    }
}

fn isToken(lhs: ?Token, rhs: Token) bool {
    if (lhs) |lhs_tok| {
        if (@intFromEnum(lhs_tok) != @intFromEnum(rhs))
            return false;

        switch (lhs_tok) {
            .literal => |lit| {
                if (@intFromEnum(lit) != @intFromEnum(rhs.literal))
                    return false;
                return switch (lit) {
                    .string => |str| std.mem.eql(u21, str, rhs.literal.string),
                    .uint => |u| u == rhs.literal.uint,
                    .int => |i| i == rhs.literal.int,
                };
            },
            .keyword => |kw| {
                return kw == rhs.keyword;
            },
            .bracket => |bracket| {
                return bracket == rhs.bracket;
            },
            .operator => |op| {
                return op == rhs.operator;
            },
            .identifier => |ident| {
                return std.mem.eql(u21, ident, rhs.identifier);
            },
        }
    }
    return false;
}

fn expectToken(got: ?Token, expects: Token) ParseError!void {
    if (got) |got_token| {
        if (!isToken(got_token, expects)) {
            return error.UnexpectedToken;
        }
    } else {
        return error.GotEof;
    }
}

fn isOneOf(lhs: ?Token, toks: []const Token) bool {
    if (lhs) |lhs_tok| {
        for (toks) |tok| {
            if (isToken(lhs_tok, tok))
                return true;
        }
    }
    return false;
}

fn expectOneOf(got: ?Token, toks: []const Token) ParseError!Token {
    if (got) |got_tok| {
        for (toks) |tok| {
            if (isToken(got_tok, tok))
                return got_tok;
        }
        return error.UnexpectedToken;
    } else {
        return error.GotEof;
    }
}

fn alloc_node(allocator: std.mem.Allocator, node: AstNode) std.mem.Allocator.Error!*AstNode {
    const new_node = try allocator.create(AstNode);
    new_node.* = node;
    return new_node;
}

fn alloc_type(allocator: std.mem.Allocator, typeinfo: TypeInfo) std.mem.Allocator.Error!*TypeInfo {
    const new_type = try allocator.create(TypeInfo);
    new_type.* = typeinfo;
    return new_type;
}
