const std = @import("std");

pub fn add_tests(
    b: *std.Build,
    target: std.Build.ResolvedTarget,
    optimize: std.builtin.OptimizeMode,
    swiftzig: *std.Build.Module,
    tests_step: *std.Build.Step,
    comptime tests: anytype,
    comptime exe_tests: anytype,
) void {
    inline for (tests) |name| {
        const new_tests = b.addTest(.{
            .target = target,
            .optimize = optimize,
            .name = std.fmt.comptimePrint("{s}_tests", .{name}),
            .root_source_file = b.path(std.fmt.comptimePrint("tests/{s}.zig", .{name})),
        });
        new_tests.root_module.addImport("swiftzig", swiftzig);
        tests_step.dependOn(&b.addRunArtifact(new_tests).step);
        tests_step.dependOn(&b.addInstallArtifact(new_tests, .{}).step);
    }
    inline for (exe_tests) |name| {
        const new_tests = b.addExecutable(.{
            .target = target,
            .optimize = optimize,
            .name = std.fmt.comptimePrint("{s}_tests", .{name}),
            .root_source_file = b.path(std.fmt.comptimePrint("tests/{s}.zig", .{name})),
        });
        new_tests.root_module.addImport("swiftzig", swiftzig);
        tests_step.dependOn(&b.addRunArtifact(new_tests).step);
        tests_step.dependOn(&b.addInstallArtifact(new_tests, .{}).step);
    }
}

pub fn build(b: *std.Build) !void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const swiftzig = b.createModule(.{ .root_source_file = b.path("src/root.zig") });
    try b.modules.put(b.dupe("swiftzig"), swiftzig);

    const tests_step = b.step("test", "Runs all the tests");
    add_tests(
        b,
        target,
        optimize,
        swiftzig,
        tests_step,
        .{ "mem", "math" },
        .{
            "start0-u8",
            "start0-void",
            "start1-u8",
            "start1-void",
            "start2-u8",
            "start2-void",
        },
    );
}
