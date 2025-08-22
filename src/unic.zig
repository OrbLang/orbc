const std = @import("std");
const codepoints = @import("codepoints");

fn compEncodeLength(comptime cdps: []const u21) usize {
    comptime var len: usize = 0;
    inline for (cdps) |cdp| {
        const chars = std.unicode.utf8EncodeComptime(cdp);
        len += chars.len;
    }
    return len;
}

pub fn compEncode(comptime cdps: []const u21) [compEncodeLength(cdps)]u8 {
    comptime var str: [compEncodeLength(cdps)]u8 = undefined;
    comptime var len: usize = 0;
    inline for (cdps) |cdp| {
        const chars = std.unicode.utf8EncodeComptime(cdp);
        const new_len = len + chars.len;
        @memcpy(str[len..new_len], chars[0..chars.len]);
        len = new_len;
    }
    return str;
}

fn compDecodeLength(comptime str: []const u8) usize {
    comptime var idx: usize = 0;
    comptime var len: usize = 0;

    // Sets a limit for iterations, to satisfy greedy compiler
    outer: inline for (0..str.len) |_| { //& 0
        if (idx >= str.len) //& false
            break;
        if (comptime codepoints.decodeAtIndex(str, idx)) |cdp| {
            idx += cdp.len;
            len += 1;
            continue :outer;
        }
        @compileError("Got invalid UTF8 string");
    }

    return len;
}

pub fn compDecode(comptime str: []const u8) [compDecodeLength(str)]u21 {
    comptime var cdps: [compDecodeLength(str)]u21 = undefined;

    comptime var idx: usize = 0;

    outer: inline for (&cdps) |*cdp| {
        if (comptime codepoints.decodeAtIndex(str, idx)) |new_cdp| {
            idx += new_cdp.len;
            cdp.* = new_cdp.code;
            continue :outer;
        }
        @compileError("Got invalid UTF8 string");
    }

    return cdps;
}

test "comptime encode" {
    const testing = std.testing;

    const encoded_simple = compEncode(&.{ 'H', 'e', 'l', 'l', 'o' });

    try testing.expectEqualStrings("Hello", &encoded_simple);
    try testing.expectEqual(5, encoded_simple.len);

    const encoded_emoji = compEncode(&.{'🦎'});

    try testing.expectEqualStrings("🦎", &encoded_emoji);
    try testing.expectEqual(4, encoded_emoji.len);

    try testing.expectEqual(encoded_emoji[0], '\xf0');
    try testing.expectEqual(encoded_emoji[1], '\x9f');
    try testing.expectEqual(encoded_emoji[2], '\xa6');
    try testing.expectEqual(encoded_emoji[3], '\x8e');
}

test "comptime decode" {
    const testing = std.testing;

    const decoded_simple = compDecode("Hello");

    try testing.expectEqualSlices(u21, &.{ 'H', 'e', 'l', 'l', 'o' }, &decoded_simple);
    try testing.expectEqual(5, decoded_simple.len);

    const decoded = compDecode("🦎");

    try testing.expectEqualSlices(u21, &.{'🦎'}, &decoded);
    try testing.expectEqual(1, decoded.len);
}
