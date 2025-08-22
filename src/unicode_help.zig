const std = @import("std");
const unicode = std.unicode;
const mem = std.mem;
const Allocator = mem.Allocator;

pub fn encode(src: []const u8, allocator: Allocator) Allocator.Error![]const u21 {
    const view: unicode.Utf8View = .initUnchecked(src);
    var iter = view.iterator();

    var out: std.ArrayList(u21) = .init(allocator);
    defer out.deinit();
    while (iter.nextCodepoint()) |codepoint| {
        try out.append(codepoint);
    }

    return try out.toOwnedSlice();
}

pub fn decode(src: []const u21, allocator: Allocator) Allocator.Error![]const u8 {
    var out: std.ArrayList(u8) = .init(allocator);
    for (src) |codepoint| {
        var buf: [4]u8 = undefined;
        const len = unicode.utf8Encode(codepoint, &buf) catch unreachable;

        try out.appendSlice(buf[0..len]);
    }

    return try out.toOwnedSlice();
}
