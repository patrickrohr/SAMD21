# Copyright Patrick Rohr, 2018

all: HostCI TargetDebug

HostCI: Workspace/HostCI
	cd $< && cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=1 ${PWD}/config/HostCI

TargetDebug: Workspace/TargetDebug
	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/board/SAMD21/toolchain_samd21g18a.cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ${PWD}/config/TargetDebug

# TargetRelease: Workspace/TargetRelease
# 	cd $< && cmake -DCMAKE_TOOLCHAIN_FILE=${PWD}/Configurations/SAMD21/toolchain_samd21g18a.cmake -DCMAKE_BUILD_TYPE=Release ${PWD}

Workspace/%:
	mkdir -p $@

.PHONY: all HostCI TargetDebug
