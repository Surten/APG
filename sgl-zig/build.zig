const std = @import("std");

pub fn build(b: *std.build.Builder) void {
    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();
    const out_dir = b.option([]const u8, "output-dir", "Library output directory");

    const lib = b.addStaticLibrary("sgl", "src/main.zig");
    lib.setBuildMode(mode);
    lib.addIncludeDir("include");
    lib.linkLibC();
    lib.bundle_compiler_rt = true;
    if (out_dir) |dir| {
        lib.setOutputDir(dir);
    }
    lib.install();

    var main_tests = b.addTest("src/main.zig");
    main_tests.setBuildMode(mode);

    const test_step = b.step("test", "Run library tests");
    test_step.dependOn(&main_tests.step);
}
