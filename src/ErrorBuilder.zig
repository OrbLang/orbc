const std = @import("std");

const ErrorBuilder = @This();

allocator: std.mem.Allocator,
list: std.ArrayList(ErrCtx),

pub fn append(self: *ErrorBuilder, loc: Loc, msg: []const u8) std.mem.Allocator.Error!void {
    try self.list.append(.{ .msg = try self.allocator.dupe(u8, msg), .loc = loc });
}

pub fn appendf(self: *ErrorBuilder, loc: Loc, fmt: []const u8, args: anytype) std.mem.Allocator.Error!void {
    const msg = try std.fmt.allocPrint(self.allocator, fmt, args);
    try self.list.append(.{ .msg = msg, .loc = loc });
}

pub fn init(allocator: std.mem.Allocator) ErrorBuilder {
    return .{
        .list = .init(allocator),
    };
}

pub fn deinit(self: ErrorBuilder) void {
    for (self.list.items) |err| {
        self.allocator.free(err.msg);
    }
    self.list.deinit();
}

pub const Loc = struct {
    line: usize,
    col: usize,
};

pub const ErrCtx = struct {
    msg: []const u8,
    loc: struct { line: usize, col: usize },
};
