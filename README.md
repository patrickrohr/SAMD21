# SAMD21 FreeRTOS Board Support Package
## Overview
Work in progress FreeRTOS BSP for ATSAMD21. This project also includes a 'barebones' build system based on the GNU toolchain. Right now, this project includes:

  - Clock Peripheral Interface
  - Generic Clock Controller Peripheral Interface


## Build
Right now, configure the project by `make`ing the following Configuration targets:

`make TargetDebug` - Builds target debug configuration

`make HostCI` - Builds host configuration including unit tests

Then navigate to the newly created `Workspace/[SELECTED_CONFIGURATION]` and build using `make`.
The BSP can be configured using `make menuconfig` using Kconfiglb and pulled in automatically via kconfiglib-cmake.

The following Configuration targets are coming soon:

`make TargetRelease` - Builds target release configuration


## Using Docker to Build

First, build and tag the docker image:

    docker build -t patrickrohr/samd21-toolchain:latest .

Then, start an interactive docker container session:

    docker run -it -v $(pwd):~/Repos/samd21-firmware patrickrohr/samd21-toolchain


## Debug on Target Using JLink

1. Start  gdb server: `JLinkGDBServer -if SWD - device ATSAMD21G18`
2. Attach gdb client: `gdb-multiarch -ex "target extended-remote :2331"
   app/hal-test/hal_test_app`

