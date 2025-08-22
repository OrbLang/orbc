const std = @import("std");
const log = @import("log.zig");

const ErrorBuilder = @This();

allocator: std.mem.Allocator,
list: std.ArrayList(Entry),
err_count: usize = 0,
warn_count: usize = 0,

pub fn init(allocator: std.mem.Allocator) ErrorBuilder {
    return .{
        .allocator = allocator,
        .list = .init(allocator),
    };
}
pub fn deinit(self: ErrorBuilder) void {
    for (self.list.items) |e| {
        self.allocator.free(e.msg);
        self.allocator.free(e.loc.file);
    }
    self.list.deinit();
}

pub fn err(self: *ErrorBuilder, loc: Loc, msg: []const u8) std.mem.Allocator.Error!void {
    const owned_msg = try self.allocator.dupe(u8, msg);
    const owned_loc: Loc = .{
        .file = try self.allocator.dupe(u8, loc.file),
        .line = loc.line,
        .col = loc.col,
    };

    try self.list.append(.{ .msg = owned_msg, .loc = owned_loc, .type = .err });
}

pub fn warn(self: *ErrorBuilder, loc: Loc, msg: []const u8) std.mem.Allocator.Error!void {
    const owned_msg = try self.allocator.dupe(u8, msg);
    const owned_loc: Loc = .{
        .file = try self.allocator.dupe(u8, loc.file),
        .line = loc.line,
        .col = loc.col,
    };

    try self.list.append(.{ .msg = owned_msg, .loc = owned_loc, .type = .warn });
}

pub fn flush(self: *ErrorBuilder) void {
    _ = self;
    log.todo(@src(), "Add error/warning formatting and printing\n", .{});
}

pub const Loc = struct {
    file: []const u8,
    line: usize,
    col: usize,
};

pub const Entry = struct {
    msg: []const u8,
    loc: Loc,
    type: enum { err, warn },
};
