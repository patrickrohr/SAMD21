# Paths
KCONFIG_DIR = ${PWD}/config/Kconfiglib
CONFIGURATION_DIR = ${PWD}/config

# Kconfig exports
export srctree = ${CONFIGURATION_DIR}
export KCONFIG_CONFIG = ${CONFIGURATION_DIR}/.config

menuconfig:
	python3 ${KCONFIG_DIR}/menuconfig.py ${CONFIGURATION_DIR}/Config.in

genconfig:
	python3 ${KCONFIG_DIR}/genconfig.py --header-path ${PWD}/peripherals/include/config.h ${CONFIGURATION_DIR}/Config.in

.PHONY: menuconfig genconfig
