# u-blox Messages Parser

## Build

```console
$ make help

Usage:
  make <target>

General
  default          When no target is specified, this help is displayed
  help             Display this help.
  conan            Install dependencies
  run              Run the executable

Building
  build            Compile both the library and the executable
  fbuild           Re-build everything from zero
  lib              Compile the (static) library

Testing
  test             Compile and run unit tests

Documentation
  pdf              Open the pdf documentation
  html             Open the documentation as a local site on the browser
  man              Open the ublox.h man page

Cleaning
  clean            Remove intermediate objects
  fclean           Remove all generated files
```

## Get started

```console
make conan
make test
```
