# SAMD21

UART Implementation on ATSAMD21G18A micro.
Or at least that's what it once was.

At this point this has become the project for all things SAMD21.

Right now, this doesn't do much, but the build system is (was :)) pretty darn clean.

You can build the project on your system using:

`make all` - Builds all configurations

`make HostCI` - Builds host configuration (Yeah, that's a dream. But maybe useful to just build libs for a unit test environment?)

`make TargetDebug` - Builds target debug configuration

`make TargetRelease` - Builds target release configuration

Things to come: Hopefully something that's actually useful :)
But for now, let me have fun with cmake already.