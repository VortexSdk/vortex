const std = @import("std");

var cwd: std.fs.Dir = undefined;
var target: std.Build.ResolvedTarget = undefined;
var optimize: std.builtin.OptimizeMode = undefined;

const DEFAULT_CLANG_OPTIONS: [6][]const u8 = [6][]const u8{
    "-std=c17",
    "-Weverything",
    "-Wno-unused-function",
    "-Wno-unsafe-buffer-usage",
    "-Wno-missing-prototypes",
    "-Wno-reserved-identifier",
};

fn add_ctest(
    b: *std.Build,
    test_step: *std.Build.Step,
    comptime subdir: []const u8,
    comptime names_len: comptime_int,
    comptime names: [names_len][]const u8,
) !void {
    inline for (names) |name| {
        const test_exe = b.addExecutable(.{
            .target = target,
            .optimize = optimize,
            .name = std.fmt.comptimePrint("{s}_test", .{name}),
        });
        test_exe.addIncludePath(.{ .path = "." });
        test_exe.addCSourceFile(.{
            .flags = switch (optimize) {
                .Debug => &DEFAULT_CLANG_OPTIONS,
                .ReleaseSafe => &(DEFAULT_CLANG_OPTIONS ++ [_][]const u8{"-O3"}),
                .ReleaseSmall => &(DEFAULT_CLANG_OPTIONS ++ [_][]const u8{"-Oz"}),
                .ReleaseFast => &(DEFAULT_CLANG_OPTIONS ++ [_][]const u8{"-Ofast"}),
            },
            .file = .{
                .path = std.fmt.comptimePrint("{s}/tests/{s}.c", .{ subdir, name }),
            },
        });
        test_step.dependOn(&b.addInstallArtifact(test_exe, .{}).step);
        test_step.dependOn(&b.addRunArtifact(test_exe).step);
    }
}

pub fn build(b: *std.Build) !void {
    cwd = std.fs.cwd();
    target = b.standardTargetOptions(.{});
    optimize = b.standardOptimizeOption(.{});

    const tests_step = b.step("test", "Runs all tests");
    try add_ctest(
        b,
        tests_step,
        "vortex",
        1,
        [_][]const u8{"mem"},
    );
    try add_ctest(
        b,
        tests_step,
        "ring",
        1,
        [_][]const u8{"ring"},
    );
}
