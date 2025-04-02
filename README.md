# u-blox Messages Parser

## Features

### As an executable

 - Display the current serial port configuration in the YAML format
 - Output the ublox messages read on the serial port in the JSON format

### As a C library

 - Iterate over all ublox messages read on the serial port
 - Verbose alternatives to termios
 - Extensive documentation

## Build



## Usage

```console
$ ./ublox_parser.exe --help
Usage: ublox_parser.exe [OPTION...] [PATH]
Prints serial port configuration and ublox messages.

  -p, --passive              Do not change the port configuration, only display
                             it.
  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Report bugs to <antoine.gagniere@orolia2s.com>.
```
