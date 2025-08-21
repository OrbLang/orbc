const std = @import("std");

pub fn main() !void {}

test "test" {
    std.testing.refAllDeclsRecursive(@This());
}
