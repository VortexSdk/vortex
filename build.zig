const std = @import("std");

pub fn add_tests(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    vortex: *std.Build.Module,
    tests_step: *std.Build.Step,
    comptime tests: anytype,
    comptime exe_tests: anytype,
) void {
    inline for (tests) |name| {
        const new_tests = b.addTest(.{
            .target = target,
            .optimize = optimize,
            .name = std.fmt.comptimePrint("{s}_{s}_tests", .{ name[0], name[1] }),
            .root_source_file = b.path(std.fmt.comptimePrint("tests/{s}/{s}.zig", .{ name[0], name[1] })),
        });
        new_tests.root_module.addImport("vortex", vortex);
        tests_step.dependOn(&b.addRunArtifact(new_tests).step);
        tests_step.dependOn(&b.addInstallArtifact(new_tests, .{}).step);
    }
    inline for (exe_tests) |name| {
        const new_tests = b.addExecutable(.{
            .target = target,
            .optimize = optimize,
            .name = std.fmt.comptimePrint("{s}_{s}_tests", .{ name[0], name[1] }),
            .root_source_file = b.path(std.fmt.comptimePrint("tests/{s}/{s}.zig", .{ name[0], name[1] })),
        });
        new_tests.root_module.addImport("vortex", vortex);
        tests_step.dependOn(&b.addRunArtifact(new_tests).step);
        tests_step.dependOn(&b.addInstallArtifact(new_tests, .{}).step);
    }
}

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const vortex = b.createModule(.{ .root_source_file = b.path("src/root.zig") });
    try b.modules.put(b.dupe("vortex"), vortex);

    const tests_step = b.step("test", "Runs all the tests");
    add_tests(
        b,
        target,
        optimize,
        vortex,
        tests_step,
        .{ .{ "mem", "mem" }, .{ "math", "math" } },
        .{
            .{ "start", "start0-u8" },
            .{ "start", "start0-void" },
            .{ "start", "start1-u8" },
            .{ "start", "start1-void" },
            .{ "start", "start2-u8" },
            .{ "start", "start2-void" },
        },
    );
}
