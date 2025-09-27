# dsa

Plain C implementations of standard **D**ata **S**tructures and **A**lgorithms.
Intended to expand the standard library with a simple library to import in
all your projects while allowing you to exclude parts when building the library.

## Build

As root
```sh
make build
make install
```

This installs both the static and shared libraries along with their headers.

This will put the library and headers under:
  - `/usr/local/lib`
  - `/usr/local/include`

Unless the PREFIX variable is changed, just ensure the paths are in the
system's `C_INCLUDE_PATH` and `LIBRARY_PATH` respectively.

And to deinstall
```sh
make deinstall
```

## Examples

To compile the examples follow the following example:

```sh
clang `pkg-config --cflags dsa` -o examples/demo_vector  examples/demo_vector.c `pkg-config --libs dsa`
```
