# SAMD21 Board Support Package
## Overview
Work in progress board support package for ATSAMD21. This project also includes a 'barebones' build system based on the GNU toolchain. Right now, this project includes:

  - Clock Peripheral Interface
  - Generic Clock Controller Peripheral Interface


## Build
Right now, configure the project by `make`ing the following Configuration target:

`make TargetDebug` - Builds target debug configuration

Then navigate to the newly created `Workspace/[SELECTED_CONFIGURATION]` and build using `make`.
The BSP can be configured using `make menuconfig` using Kconfiglb and pulled in automatically via kconfiglib-cmake.

The following Configuration targets are coming soon:

`make all` - Builds all configurations

`make TargetDebug` - Builds target debug configuration

`make TargetRelease` - Builds target release configuration

`make HostCI` - Builds host configuration (Yeah, that's a dream. But maybe useful to just build libs for a unit test environment?)

## Design
### Peripheral Interface Considerations
The peripheral interface was designed to provide a simple hardware abstraction for use in the drivers (coming soon). All peripheral interfaces are kept OS independent, and are supporting (or will support) callback interfaces via function pointers.
