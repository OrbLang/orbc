const std = @import("std");
const log = @import("../log.zig");
const unic = @import("../unic.zig");
const parsing = @import("../parsing.zig");

const SpellType = parsing.AstNode.SpellType;
const IdentTypePair = parsing.AstNode.IdentTypePair;

pub const TypeInfo = union(enum) {
    pub const Prim = enum {
        void, // TODO: Better void name
        rune,
        r64,
        r32,
        r16,
        r8,
        flow,
        f32,
        sigil,
        glyph,
        chant,

        pub fn fromStr(str: []const u21) error{InvalidStr}!Prim {
            inline for (std.meta.fields(Prim)) |field| {
                const field_name = &unic.compDecode(field.name);
                if (std.mem.eql(u21, field_name, str))
                    return @enumFromInt(field.value);
            }
            return error.InvalidStr;
        }
    };

    pub const Pointer = union(enum) {
        ptr: struct { inner: *TypeInfo, is_const: bool },
        slice: struct { inner: *TypeInfo, is_const: bool },
        array: struct { inner: *TypeInfo, len: usize, is_const: bool },

        pub fn deinit(self: Pointer, allocator: std.mem.Allocator) void {
            switch (self) {
                .ptr => |data| {
                    data.inner.deinit(allocator);
                    allocator.destroy(data.inner);
                },
                .slice => |data| {
                    data.inner.deinit(allocator);
                    allocator.destroy(data.inner);
                },
                .array => |data| {
                    data.inner.deinit(allocator);
                    allocator.destroy(data.inner);
                },
            }
        }
    };

    pub const Spell = struct {
        spelltype: SpellType,
        components: []TypeInfo,
        ret_type: *TypeInfo,

        pub fn deinit(self: Spell, allocator: std.mem.Allocator) void {
            for (self.components) |comp|
                comp.deinit(allocator);

            allocator.free(self.components);

            self.ret_type.deinit(allocator);
            allocator.destroy(self.ret_type);
        }
    };

    pub const Codex = struct {
        members: []const []const u21,
        decls: []Spell,

        pub fn deinit(self: Codex, allocator: std.mem.Allocator) void {
            for (self.members) |member|
                allocator.free(member);

            allocator.free(self.members);

            for (self.decls) |decl|
                decl.deinit(allocator);

            allocator.free(self.decls);
        }
    };
    pub const Artifact = struct {
        fields: []IdentTypePair,
        decls: []Spell,

        pub fn deinit(self: Artifact, allocator: std.mem.Allocator) void {
            for (self.fields) |field| {
                allocator.free(field.ident);
                field.typeinfo.deinit(allocator);
            }
            allocator.free(self.fields);

            for (self.decls) |decl|
                decl.deinit(allocator);

            allocator.free(self.decls);
        }
    };

    prim: Prim,
    ptr: Pointer,
    spell: Spell,

    codex: Codex,
    artifact: Artifact,

    alias: []const u21,

    pub fn deinit(self: TypeInfo, allocator: std.mem.Allocator) void {
        switch (self) {
            .prim => {},
            .ptr => |ptr| {
                ptr.deinit(allocator);
            },
            .spell => |spell| {
                spell.deinit(allocator);
            },
            .codex => |codex| {
                codex.deinit(allocator);
            },
            .artifact => |artifact| {
                artifact.deinit(allocator);
            },
            .alias => |alias| allocator.free(alias),
        }
    }

    pub const GetSizeErr = error{ FoundSpell, FoundAlias };

    pub fn getSize(self: TypeInfo) GetSizeErr!usize {
        switch (self) {
            .prim => |prim| {
                return switch (prim) {
                    .void => 0,
                    .rune => 8, // TODO: Get system size
                    .r64 => 8,
                    .r32 => 4,
                    .r16 => 2,
                    .r8 => 1,

                    .flow => 8,
                    .f32 => 4,

                    .sigil => 1,
                    .glyph => 4,
                    .chant => log.todo(@src(), "Figure out wtf a chant is\n", .{}),
                };
            },
            .ptr => |ptr| {
                return switch (ptr) {
                    .ptr => 8,
                    .slice, .array => 16, // TODO: Does an array have to keep its size?
                };
            },
            .spell => return error.FoundSpell,
            .codex => |codex| {
                return if (codex.members.len <= 256) 1 else 2; // 2^16 should be enough lol
            },
            .artifact => |artifact| {
                var sum: usize = 0;
                for (artifact.fields) |field| {
                    sum += try field.typeinfo.getOffset();
                }
                return sum;
            },
            .alias => return error.FoundAlias,
        }
    }

    pub fn getOffset(self: TypeInfo) GetSizeErr!usize {
        // TODO: Get proper offset
        return try self.getSize();
    }
};
