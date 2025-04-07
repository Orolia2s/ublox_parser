# u-blox Messages Parser

## Features

### As an executable

 - Display the current serial port configuration in the YAML format
 - Output the ublox messages read on the serial port in the JSON format

### As a C library

 - Iterate over all ublox messages read on the serial port
 - Verbose alternatives to termios
 - Extensive documentation

## Usage

### Use in your project

Add the dependency in your `build.zig.zon` by running the following command:
```shell
zig fetch --save git+https://github.com/Orolia2s/ublox_parser#master
```

Then, in your `build.zig`:
```zig
const ublox = b.dependency("ublox_parser", { .target = target, .optimize = optimize }).artifact("ublox_parser");

// wherever needed:
exe.linkLibrary(ublox);
```

### Build

requires [zig](https://ziglang.org/download/) >= 0.14.0

```shell
zig build
```

### Test

```shell
zig build test
```

### Run

```console
$ zig build run -- --help
Usage: ublox_parser [OPTION...] [PATH]
Prints serial port configuration and ublox messages.

  -f, --file                 Input is not a serial port but a file. Defaults to
                             false
  -p, --passive              Do not change the port configuration, only display
                             it. Defaults to false
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Report bugs to <antoine.gagniere@orolia2s.com>.
```

### Generate documentation

```shell
zig build doc
open doc/html/index.html
```
