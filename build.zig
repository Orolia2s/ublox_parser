const std = @import("std");

// Waiting for https://github.com/ziglang/zig/pull/20510
const VERSION = "0.2.0";

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{ .preferred_optimize_mode = .ReleaseSafe });
    const target = b.standardTargetOptions(.{});
    const lib = b.addStaticLibrary(.{
        .name = "ublox_parser",
        .target = target,
        .optimize = optimize,
    });

    const libo2s = b.dependency("libo2s", .{ .target = target, .optimize = optimize }).artifact("o2s");
    const blackmagic = b.dependency("blackmagic", .{});

    lib.addCSourceFiles(.{
        .root = b.path("src"),
        .files = &.{
            "checksum.c",
            "globals.c",
            "header.c",
            "monitoring_hardware.c",
            "monitoring_rf.c",
            "position_time.c",
            "reader.c",
            "reader_loop.c",
            "receive.c",
            "receiver_manager.c",
            "subscribe.c",
            "serial_print_config.c",
        },
    });
    lib.addIncludePath(b.path("include"));
    lib.addIncludePath(blackmagic.path("include"));
    lib.installHeadersDirectory(b.path("include"), "ublox_parser", .{});
    lib.linkLibC();
    lib.linkLibrary(libo2s);
    b.installArtifact(lib);

    { // Run
        const run_step = b.step("run", "Run the app");
        const exe = b.addExecutable(.{
            .name = "ublox_parser",
            .target = target,
            .optimize = optimize,
        });
        const run_cmd = b.addRunArtifact(exe);

        exe.addCSourceFile(.{ .file = b.path("parse_ublox.c") });
        exe.linkLibrary(lib);
        exe.linkLibrary(libo2s);
        exe.addIncludePath(blackmagic.path("include"));
        if (b.args) |args| {
            run_cmd.addArgs(args);
        }
        run_step.dependOn(&run_cmd.step);
    }
    { // Test
        const test_step = b.step("test", "Run unit tests");
        const unit_tests = b.addExecutable(.{
            .name = "test_ublox_parser",
            .target = target,
            .optimize = optimize,
        });
        const run_unit_tests = b.addRunArtifact(unit_tests);
        const libft = b.dependency("libft", .{ .target = target, .optimize = optimize, .@"libunit-long-output" = true });

        unit_tests.addCSourceFiles(.{ .root = b.path("test"), .files = &.{ "checksum.c", "main.c", "message_size.c", "serial.c", "parse.c" } });
        unit_tests.linkLibrary(lib);
        unit_tests.linkLibrary(libo2s);
        unit_tests.linkLibrary(libft.artifact("ft"));
        unit_tests.linkLibrary(libft.artifact("unit"));
        unit_tests.addIncludePath(b.path("include"));
        unit_tests.addIncludePath(blackmagic.path("include"));
        unit_tests.addIncludePath(libft.path("test/framework"));
        unit_tests.addIncludePath(libft.path("include")); // ugly fix
        test_step.dependOn(&run_unit_tests.step);
    }
    { // Documentation
        const doc_step = b.step("doc", "Generate the documentation as a static website");
        const run_doxygen = b.addSystemCommand(&.{"doxygen"});

        run_doxygen.addFileArg(b.path("doc/Doxyfile"));
        run_doxygen.setEnvironmentVariable("PROJECT_VERSION", VERSION);
        doc_step.dependOn(&run_doxygen.step);
    }
}
