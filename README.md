# SAMD21

Work in progress board support package for ATSAMD21. This project also includes a 'barebones' build system based on the GNU toolchain.

You can build the project by `make`ing one of the following Configuration targets:

`make all` - Builds all configurations

`make TargetDebug` - Builds target debug configuration

`make TargetRelease` - Builds target release configuration

`make HostCI` - Builds host configuration (Yeah, that's a dream. But maybe useful to just build libs for a unit test environment?)

Then navigate to the newly created `Workspace/[SELECTED_CONFIGURATION]` and build using `make`.
