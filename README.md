# u-blox Messages Parser

## Build

```console
$ make help

This Makefile allows one to build, run and test this ublox parser.

Usage:
  make <target>

General:
  default          When no target is specified, this help is displayed
  help             Display this help.
  raw_help         Display the help without color
  version          Display the project's version
  info             Print the project's name, version, copyright notice and author

Building:
  build            Compile both the library and the executable
  fbuild           Re-build everything from zero
  lib              Compile the (static) library

Run:
  run              Run the executable
  test             Compile and run unit tests

Documentation:
  doc              Generate all the documentation
  pdf              Generate and open the pdf documentation
  html             Generate and open the documentation as a local site on the browser
  man              Generate and open the ublox.h man page

Cleaning:
  clean            Remove intermediate objects
  fclean           Remove all generated files
```

## Getting started

Setup your environment:

```bash
./setup.sh
```

You can now build and run the code, try:

```
make test
```
