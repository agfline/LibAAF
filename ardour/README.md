# Ardour AAF Import

**ardour_aafimport** allows to create an ardour session and import an AAF file into it.

## Build

You must first build and install `libaaf`, download and build [ardour 6 source](https://github.com/Ardour/ardour).

Then ensure that those two variables in `Makefile` point to ardour6 source and libs.

```
ARDOUR_INCLUDE_PATH = /usr/local/include/ardour6
ARDOUR_LIB_PATH = /usr/local/lib/ardour6
```

Run `make ardour`

Then you can run `./bin/ardour_aafimport` which is a shell wrapper for `./bin/_ardour_aafimport` binary.

## Usage

```
./bin/ardour_aafimport --help
./bin/ardour_aafimport --template <name> --session-path /path/to/session --aaf /path/tofile.aaf
```
