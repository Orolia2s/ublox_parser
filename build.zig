const std = @import("std");

pub fn build(b: *std.Build) void {
    const optimize = b.standardOptimizeOption(.{ .preferred_optimize_mode = .ReleaseSafe });
    const target = b.standardTargetOptions(.{});
    const lib = b.addStaticLibrary(.{
        .name = "ublox_parser",
        .target = target,
        .optimize = optimize,
    });

    const libft = b.dependency("libft", .{ .target = target, .optimize = optimize });
    const logc = b.dependency("logc", .{ .target = target, .optimize = optimize });

    lib.addCSourceFiles(.{
        .root = b.path("src/file"),
        .files = &.{ "open.c", "close.c", "read.c" },
    });
    lib.addCSourceFiles(.{
        .root = b.path("src/serial"),
        .files = &.{ "baudrate.c", "close.c", "get_options.c", "make_raw.c", "open.c", "port_config.c", "print.c" },
    });
    lib.addCSourceFiles(.{
        .root = b.path("src/ublox"),
        .files = &.{ "checksum.c", "globals.c", "header.c", "monitoring_hardware.c", "monitoring_rf.c", "position_time.c", "reader.c", "reader_loop.c", "receive.c", "receiver_manager.c", "subscribe.c" },
    });
    lib.addIncludePath(b.path("include"));
    lib.installHeadersDirectory(b.path("include"), "ublox_parser", .{});
    lib.linkLibC();
    lib.linkLibrary(libft.artifact("ft"));
    lib.addIncludePath(libft.path("include")); // ugly fix
    b.installArtifact(lib);

    lib.addCSourceFile(.{ .file = logc.path("src/log.c") });
    lib.addIncludePath(logc.path("src"));

    { // Run
        const run_step = b.step("run", "Run the app");
        const exe = b.addExecutable(.{
            .name = "ublox_parser",
            .target = target,
            .optimize = optimize,
        });
        const run_cmd = b.addRunArtifact(exe);

        exe.addCSourceFile(.{ .file = b.path("parse_ublox.c") });
        exe.addIncludePath(libft.path("include")); // ugly fix
        exe.linkLibrary(lib);
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

        unit_tests.addCSourceFiles(.{ .root = b.path("test"), .files = &.{ "baudrate.c", "checksum.c", "main.c", "message_size.c", "port_config.c", "serial.c" } });
        unit_tests.linkLibrary(lib);
        unit_tests.linkLibrary(libft.artifact("unit"));
        unit_tests.addIncludePath(b.path("include"));
        unit_tests.addIncludePath(libft.path("include")); // ugly fix
        unit_tests.addIncludePath(libft.path("test/framework"));
        test_step.dependOn(&run_unit_tests.step);
    }
}
