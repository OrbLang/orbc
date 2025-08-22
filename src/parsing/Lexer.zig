//! This is a Lexer that turns source code into Tokens
//! It only reads up to 4096 bytes of source code at a time to save memory
//! In order to avoid excessive memory management we avoid using allocators and instead save identifiers in a threadlocal buffer
//! Therefore all identifiers which are returned from the Lexer are invalidated on the next call

const std = @import("std");
const builtin = @import("builtin");
const mem = std.mem;
const unicode = std.unicode;

const unicode_help = @import("../unicode_help.zig");

const File = std.fs.File;
const Allocator = mem.Allocator;

const Properties = @import("Properties");
const codepoints = @import("codepoints");
const Lexer = @This();

/// A buffer where we store 4096 bytes of a file at a time,
/// we do this to minimize memory usage while also not making a billion syscalls to read from the source file
threadlocal var buf: [4096]u8 = undefined;
threadlocal var identifier_buf: [512]u21 = undefined;

file_reader: File.Reader,
iter: codepoints.Iterator,
props: Properties,

pub const Token = union(enum) {
    pub const Literal = union(enum) {
        chant: []u21,
        urune: usize,
        rune: isize,
    };
    pub const Keyword = enum {
        create,
        spell,
        cast,
        artifact,
        codex,
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
    /// Is stored in a threadlocal buffer and is invalidated on the next call
    identifier: []u21,
};

// Init
pub fn init(reader: File.Reader, props: Properties) Lexer {
    return .{
        .file_reader = reader,
        .iter = .init(&.{}),
        .props = props,
    };
}

pub const LexerNextError = error{HugeIdentifier};

/// Get the next Token and advance the internal iterator
/// Identifiers returned by this function are invalidated on the next call
pub fn next(self: *Lexer) LexerNextError!?Token {
    const begin = self.peekCodePoint() orelse return null;
    var i: std.meta.Int(.unsigned, std.math.log2(identifier_buf.len)) = 0;

    // Parse identifiers and keywords
    if (self.props.isXidStart(begin.code)) {
        identifier_buf[i] = begin.code;
        i += 1;
        self.iter.i += begin.len;

        while (self.peekCodePoint()) |codepoint| {
            // If the identifier_buf is about to overflow then we error instead
            if (i == std.math.maxInt(@TypeOf(i))) {
                return LexerNextError.HugeIdentifier;
            }

            if (self.props.isXidContinue(codepoint.code)) {
                identifier_buf[i] = codepoint.code;
                i += 1;

                self.iter.i += codepoint.len;
            } else {
                break;
            }
        }

        // Does the identifier match a keyword?
        const keywords: []const std.builtin.Type.EnumField = comptime std.meta.fields(Token.Keyword);
        inline for (keywords) |keyword| {
            const decoded = unicode_help.decode(identifier_buf[0..i], std.heap.smp_allocator) catch unreachable;

            if (std.mem.eql(u8, keyword.name, decoded)) {
                return Token{ .keyword = @enumFromInt(keyword.value) };
            }
        }

        // If it doesn't match a keyword return the identifier.
        return Token{ .identifier = identifier_buf[0..i] };
    }

    if (self.props.isWhitespace(begin.code)) {
        self.iter.i += begin.len;
        while (self.peekCodePoint()) |codepoint| {
            if (self.props.isWhitespace(codepoint.code)) {
                self.iter.i += codepoint.len;
                continue;
            }

            break;
        }

        return self.next();
    }

    return null;
}

/// Peek the next Token without advancing the internal iterator
/// Identifiers returned by this function are invalidated on the next call
pub fn peek(self: Lexer) LexerNextError!?Token {
    // --BENCHMARK--
    // Maybe make the function non const so it doesn't copy the lexer
    // Benchmark this at some point...
    return self.next();
}

fn peekCodePoint(self: *Lexer) ?codepoints.CodePoint {
    // The bytes we have yet to scan in buf
    const bytes_remaining = self.iter.bytes.len - self.iter.i;

    // If there are no more bytes left in the buffer we load completely new bytes into the buffer
    if (bytes_remaining == 0) {
        // --BENCHMARK--
        // @branchHint(.unlikely)

        const num_bytes = self.file_reader.read(buf[0..]) catch return null;
        self.iter.bytes = buf[0..num_bytes];
        self.iter.i = 0;
    }

    // The max length a unicode codepoint can have is 4 bytes.
    // Therefore in order to avoid cutting a codepoint in half, if there are less than 4 bytes remaining,
    // we move them to the beginning, and fill in the rest of the buffer untill there are no more characters in the file.
    else if (bytes_remaining < 4) {
        // --BENCHMARK--
        // @branchHint(.unlikely)
        const tail = self.iter.bytes[self.iter.i..];
        @memcpy(buf[0..bytes_remaining], tail);

        const num_bytes = self.file_reader.read(buf[bytes_remaining..]) catch return null;

        // If there are no more bytes to be read, don't modify the iterator
        if (num_bytes == 0) {
            return self.iter.peek();
        }
        self.iter.bytes = buf[0 .. bytes_remaining + num_bytes];
        self.iter.i = 0;
    }

    return self.iter.peek();
}

const testing = std.testing;

test "Parsing simple file" {
    const file = try std.fs.cwd().openFile("testing/lexingSimpleFile.orb", .{});
    defer file.close();

    const props: Properties = try .init(testing.allocator);
    defer props.deinit(testing.allocator);

    var lexer: Lexer = .init(file.reader(), props);

    const hello = try unicode_help.encode("hello", testing.allocator);
    defer testing.allocator.free(hello);

    const there = try unicode_help.encode("there", testing.allocator);
    defer testing.allocator.free(there);

    try std.testing.expectEqual(try lexer.next(), Lexer.Token{ .keyword = .create });
    try std.testing.expectEqualSlices(u21, (try lexer.next()).?.identifier, hello);
    try std.testing.expectEqualSlices(u21, (try lexer.next()).?.identifier, there);
    try std.testing.expectEqual(try lexer.next(), Lexer.Token{ .keyword = .spell });
    try std.testing.expectEqual(null, try lexer.next());
}
