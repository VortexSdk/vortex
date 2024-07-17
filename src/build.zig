const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "vortex",
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });

    b.installArtifact(exe);

    if (optimize != .Debug) {
        exe.root_module.strip = true;
        if (target.result.os.tag != .macos) {
            exe.want_lto = true;
        }
    }

    const run_step = b.step("run", "Run the application");
    run_step.dependOn(&b.addRunArtifact(exe).step);

    const test_step = b.step("test", "Runs the tests");
    const root_mod = b.addModule("vortex", .{ .root_source_file = std.Build.LazyPath.relative("src/root.zig") });
    const tests = b.addTest(.{ .root_source_file = std.Build.LazyPath.relative("tests/lexer/lexer.zig") });
    tests.root_module.addImport("vortex", root_mod);
    test_step.dependOn(&b.addRunArtifact(tests).step);
}
