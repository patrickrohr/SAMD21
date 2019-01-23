# Copyright Patrick Rohr, 2018

all: HostCI TargetDebug TargetRelease

HostCI: Workspace/HostCI
	cd $< && cmake -DCMAKE_BUILD_TYPE=Debug ${PWD}

TargetDebug: Workspace/TargetDebug
	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/Configurations/SAMD21/toolchain_samd21g18a.cmake -DCMAKE_BUILD_TYPE=Debug ${PWD}

TargetRelease: Workspace/TargetRelease
	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/Configurations/SAMD21/toolchain_samd21g18a.cmake -DCMAKE_BUILD_TYPE=Release ${PWD}

Workspace/%:
	mkdir -p $@

.PHONY: all HostCI TargetDebug TargetRelease
