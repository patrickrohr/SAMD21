# Copyright Patrick Rohr, 2018

all: HostCI TargetDebug TargetRelease

HostCI: Workspace/HostCI
	cd $< && cmake -DDEBUG=1 ${PWD}

TargetDebug: Workspace/TargetDebug
	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/Configurations/SAMD21/toolchain_samd21g18a.cmake -DDEBUG=1 ${PWD}

TargetRelease: Workspace/TargetRelease
	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/Configurations/SAMD21/toolchain_samd21g18a.cmake ${PWD}

Workspace/%:
	mkdir -p $@

.PHONY: all HostCI TargetDebug TargetRelease