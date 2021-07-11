# dread
NFC Scanner with Mobile Network gateway.  

The scanner reads from Mifare Cards and uploads it via FTP connection to a storage service.

# Overview

# Architecture

There are 3 hw models using AVR hw. Currently this repo only handles atmega328P architecture. Further implementation is needed for SAMD20.


# Setup

## Linux Tools

For host and unit testing:
`sudo apt install build-essentials libgtest-dev libgmock-dev`

For cross compilation targets:
`sudo apt install gcc-avr g++-avr`


## Git Submodules

Ensure to have ssh or change .gitmodules to https url's

```
git submodule init
git submodule update
```

# Compile

Compilation uses build repo, which is a simple custom make generator to be able to compile multiple flavors with different include configurations.
It's not as sophisticated as autotools but is able to isolate builds and code. 

The compilation have 4 flavors:  

- **slv**: Slave architecture using avr cross-compilation.
- **mst**: Master architecture using avr cross-compilation.
- **std**: Standalone architecture using avr cross-compilation.
- **host**: Native compilation to use *googletest* for unit testing.

## Compilation Steps
```
make slv
make mst
make std
make host
make all
```

## Cleanup Steps
```
make clean-slv
make clean-mst
make clean-std
make clean-host
make clean-all
```

# Unit Testing
The unit-testing uses `make host` compilation.
Run unittests:

```
${build_dir}/host/bin/unittest
```

# Qemu
Emulations are pending

# Cross Compilation Support

