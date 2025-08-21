const std = @import("std");

const AstNode = @This();

inner: AstNodeInner,
typeinfo: TypeInfo,

pub fn deinit(self: AstNode, allocator: std.mem.Allocator) void {
    self.inner.deinit(allocator);
    self.typeinfo.deinit(allocator);
}

pub const AstLiteral = union(enum) {
    rune: isize,
    r64: i64,
    r32: i32,
    r16: i16,
    r8: i8,

    flow: f64,
    f32: f32,

    sigil: bool,

    glyph: u21,
    chant: []const u21,

    codex_member: []const u21,
    artifact: []struct { ident: []const u21, typeinfo: TypeInfo },

    pub fn deinit(self: AstLiteral, allocator: std.mem.Allocator) void {
        switch (self) {
            .chant => |cha| allocator.free(cha),
            .codex_member => |cdx| allocator.free(cdx),
            .artifact => |fields| {
                for (fields) |field| {
                    allocator.free(field.ident);
                    field.typeinfo.deinit(allocator);
                }
            },
            _ => {},
        }
    }
};

pub const Symbol = enum {
    add,
    sub,
    times,
    div,
    mod,

    access_member,
    static_access_member,

    deref,
    address,
};

pub const SpellType = enum {
    spell,
    cantrip,
};

pub const Qualifier = enum {
    @"pub",
};

pub const TypeInfo = struct {
    pub fn deinit(self: TypeInfo, allocator: std.mem.Allocator) void {
        _ = self;
        _ = allocator;
    }
};

pub const AstRoot = struct {
    allocator: std.mem.Allocator,
    nodes: []AstNode,

    pub fn deinit(self: AstRoot) void {
        for (self.nodes) |node|
            node.deinit(self.allocator);

        self.allocator.free(self.nodes);
    }
};

pub const AstNodeInner = union(enum) {
    binop: struct {
        lhs: *AstNode,
        op: Symbol,
        rhs: *AstNode,
    },
    unop: struct {
        operand: *AstNode,
        op: Symbol,
    },
    literal: AstLiteral,
    read_var: []const u21,
    /// Ready a spell, by e.g. `foo(1, 2)`.
    /// This will call the spell if its a cantrip
    ready_spell: struct {
        spell: *AstNode,
        params: []AstNode,
    },
    decl_var: struct {
        ident: []const u21,
        is_const: bool,
        value: ?*AstNode,
    },
    /// Declare a spell
    decl_spell: struct {
        spelltype: SpellType = .spell,
        qualifiers: []Qualifier,
        ident: []const u21,
        params: []struct { ident: []const u21, typeinfo: TypeInfo },
        body: *AstNode,
    },
    /// A block of nodes
    block: struct {
        label: ?[]const u21,
        nodes: []AstNode,
    },
    @"return": *AstNode,
    @"break": struct {
        label: ?[]const u21,
        value: ?*AstNode,
    },
    @"continue": struct {
        label: ?[]const u21,
    },
    /// A cast block, for casting spells. Like `cast foo(1, 2)`
    cast: *AstNode,
    /// Import a `.orb` file from `.path` as `.ident`
    import: struct {
        ident: []const u21,
        path: []const u21,
    },

    pub fn deinit(self: AstNodeInner, allocator: std.mem.Allocator) void {
        switch (self) {
            .binop => |data| {
                data.lhs.deinit(allocator);
                allocator.destroy(data.lhs);

                data.rhs.deinit(allocator);
                allocator.destroy(data.rhs);
            },
            .unop => |data| {
                data.operand.deinit(allocator);
                allocator.destroy(data.operand);
            },
            .literal => |lit| {
                lit.deinit(allocator);
            },
            .read_var => |ident| {
                allocator.free(ident);
            },
            .ready_spell => |data| {
                data.spell.deinit(allocator);
                allocator.destroy(data.spell);

                for (data.params) |param|
                    param.deinit(allocator);

                allocator.free(data.params);
            },
            .decl_var => |data| {
                allocator.free(data.ident);
                if (data.value) |val| {
                    val.deinit(allocator);
                    allocator.destroy(val);
                }
            },
            .decl_spell => |data| {
                allocator.free(data.ident);
                data.body.deinit(allocator);
                allocator.destroy(data.body);

                allocator.free(data.qualifiers);

                for (data.params) |param|
                    param.deinit(allocator);

                allocator.free(data.params);
            },
            .block => |data| {
                if (data.label) |label|
                    allocator.free(label);

                for (data.nodes) |node|
                    node.deinit(allocator);

                allocator.free(data.nodes);
            },
            .@"return" => |val| {
                val.deinit(allocator);
                allocator.destroy(val);
            },
            .@"break" => |data| {
                if (data.label) |label|
                    allocator.free(label);

                if (data.value) |val| {
                    val.deinit(allocator);
                    allocator.destroy(val);
                }
            },
            .@"continue" => |data| {
                if (data.label) |label|
                    allocator.free(label);
            },
            .cast => |node| {
                node.deinit(allocator);
                allocator.destroy(node);
            },
            .import => |data| {
                allocator.free(data.ident);
                allocator.free(data.path);
            },
        }
    }

    pub fn isTopLevelDef(self: AstNodeInner) bool {
        return switch (self) {
            .decl_var, .decl_spell, .block, .import => true,
            .binop, .unop, .literal, .read_var, .ready_spell, .@"return", .@"break", .@"continue", .cast => false,
        };
    }
};
