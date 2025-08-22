const CLEAR: []const u8 = "\x1b[0m";
const RED: []const u8 = "\x1b[31m";
const GREEN: []const u8 = "\x1b[32m";
const YELLOW: []const u8 = "\x1b[33m";
const DIM: []const u8 = "\x1b[2m";

/// Write a log message with the supplied logtype.
/// This will only have an effect if the global loglevel allows it.
pub fn write(comptime logtype: LogType, msg: []const u8) if (logtype != .fatal) void else noreturn {
    if (loglevel_.includes(logtype.toLogLevel()))
        logtype.getOutStream().writer().print(logtype.colored(false, "{s}"), .{msg}) catch {};

    if (logtype == .fatal)
        std.process.exit(1);
}

// Print a log message with the supplied logtype.
/// This will only have an effect if the global loglevel allows it.
pub fn print(comptime logtype: LogType, comptime fmt: []const u8, args: anytype) if (logtype == .fatal) noreturn else void {
    if (loglevel_.includes(logtype.toLogLevel()))
        logtype.getOutStream().writer().print(logtype.colored(false, fmt), args) catch {};

    if (logtype == .fatal)
        std.process.exit(1);
}

/// Write a verbose log message with the supplied logtype.
/// This will only have an effect if the global loglevel is `.verbose`
pub fn writeVerbose(comptime logtype: LogType, msg: []const u8) if (logtype != .fatal) void else noreturn {
    if (loglevel_.includes(.verbose))
        logtype.getOutStream().writer().print(logtype.colored(false, "{s}"), .{msg}) catch {};

    if (logtype == .fatal)
        std.process.exit(1);
}

/// Print a verbose log message with the supplied logtype.
/// This will only have an effect if the global loglevel is `.verbose`
pub fn printVerbose(comptime logtype: LogType, comptime fmt: []const u8, args: anytype) if (logtype == .fatal) noreturn else void {
    if (loglevel_.includes(.verbose))
        logtype.getOutStream().writer().print(logtype.colored(false, fmt), args) catch {};

    if (logtype == .fatal)
        std.process.exit(1);
}

/// Prints source info and the message supplied, and then exits the program.
/// Use the `@src()` builtin for the first argument.
/// This is only meant for debug builds, and should not be included in releases.
pub fn todo(comptime src: std.builtin.SourceLocation, comptime fmt: []const u8, args: anytype) noreturn {
    if (@import("builtin").mode != .Debug) {
        const todo_err_msg = std.fmt.comptimePrint("[{s}.{s}: {s}:{}:{}] log.todo() calls are not allowed in release builds\n", .{ src.module, src.fn_name, src.file, src.line, src.column });
        @compileError(todo_err_msg);
    }

    std.io.getStdErr().writer().print(
        RED ++ "[TODO] {s}.{s}: {s}:{}:{}: " ++ fmt ++ CLEAR,
        .{ src.module, src.fn_name, src.file, src.line, src.column } ++ args,
    ) catch {};
    std.process.exit(2);
}

/// Redirect all log messages from stdout to a file.
/// A value of `null` will ignore a redirect.
/// This only effects log messages meant for stdout,
/// log messages to stderr will still be printed there, unless specified with `redirectStdErr()`
pub fn redirectStdOut(to: ?std.fs.File) void {
    redirect_.stdout = to;
}

/// Redirect all log messages from stderr to a file.
/// A value of `null` will ignore a redirect.
/// This only effects log messages meant for stderr,
/// log messages meant forstdout will still be printed there, unless specified with `redirectStdOut()`
pub fn redirectStdErr(to: ?std.fs.File) void {
    redirect_.stderr = to;
}

/// Set the global loglevel that will be used by all logging functions
pub fn setLogLevel(loglevel: LogLevel) void {
    loglevel_ = loglevel;
}

/// Get the global loglevel
pub fn getLogLevel() LogLevel {
    return loglevel_;
}

var loglevel_: LogLevel = .info;

var redirect_: struct { stdout: ?std.fs.File = null, stderr: ?std.fs.File = null } = .{};

/// Get StdOut, or a redirect file if one is given
fn get_stdout() std.fs.File {
    if (redirect_.stdout) |stdout| {
        return stdout;
    }
    return std.io.getStdOut();
}

/// Get StdErr, or a redirect file if one is given
fn get_stderr() std.fs.File {
    if (redirect_.stderr) |stderr| {
        return stderr;
    }
    return std.io.getStdErr();
}

/// Gets a writer that writes log messages with a given `LogType`
pub fn LogWriter(comptime logtype: LogType, is_verbose: bool) type {
    const print_func = if (is_verbose) printVerbose else print;
    const write_func = if (is_verbose) writeVerbose else write;

    const Writer = struct {
        const Self = @This();
        pub fn print(comptime fmt: []const u8, args: anytype) !(if (logtype != .fatal) void else noreturn) {
            print_func(logtype, fmt, args);
        }

        pub fn write(bytes: []const u8) !(if (logtype != .fatal) void else noreturn) {
            write_func(logtype, bytes);
        }
    };

    return Writer;
}

/// The `LogLevel` decides which kinds of log messages can be printed.
pub const LogLevel = enum {
    const Self = @This();

    /// Allow no log messages to be printed
    none,
    /// Only allow fatal error messages to be printed
    fatal,
    /// Allows errors and fatal error messages to be printed
    err,
    /// Allows warnings, errors, and fatal error messages to be printed
    warn,
    /// Allows info, warnings, errors, and fatal error messages to be printed
    info,
    /// Allows all logs, and verbose log messages to be printed
    verbose,

    /// Check if a loglevel permits a log message to be printed
    pub fn includes(self: Self, lvl: Self) bool {
        return @intFromEnum(self) >= @intFromEnum(lvl);
    }
};

pub const LogType = enum {
    const Self = @This();

    fatal,
    err,
    warn,
    ok,
    info,

    /// Converts a `LogType` to its `LogLevel`.
    /// This is used to check if a loglevel permits a given logtype to be printed
    pub fn toLogLevel(comptime self: Self) LogLevel {
        return switch (self) {
            .fatal => .fatal,
            .err => .err,
            .warn => .warn,
            .ok => .info,
            .info => .info,
        };
    }

    /// Get the outstream that the log message should be printed to.
    pub fn getOutStream(comptime self: Self) std.fs.File {
        return switch (self) {
            .fatal, .err, .warn, .ok => get_stderr(),
            .info => get_stdout(),
        };
    }

    fn colored(comptime self: Self, comptime verbose: bool, msg: []const u8) []const u8 {
        // Set to dim if this is a verbose message
        const style = if (verbose) DIM else "";

        const prefix = comptime switch (self) {
            .fatal => style ++ RED ++ "[FATAL]: ",
            .err => style ++ RED ++ "[ERR]: ",
            .warn => style ++ YELLOW ++ "[WARN]: ",
            .ok => style ++ GREEN ++ "[OK]: ",
            .info => "[INFO]: ",
        };

        return prefix ++ msg ++ CLEAR;
    }
};

const std = @import("std");
